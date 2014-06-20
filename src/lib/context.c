/*   
 * This file is part of cf4ocl (C Framework for OpenCL).
 * 
 * cf4ocl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or (at your option) any later version.
 * 
 * cf4ocl is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with cf4ocl. If not, see 
 * <http://www.gnu.org/licenses/>.
 * */
 
/** 
 * @file
 * @brief OpenCL context wrapper.
 * 
 * @author Nuno Fachada
 * @date 2014
 * @copyright [GNU Lesser General Public License version 3 (LGPLv3)](http://www.gnu.org/licenses/lgpl.html)
 * */
 
#include "context.h"

/**
 * Context wrapper object.
 */
struct cl4_context {
	
	/* Platform. */
	CL4Platform* platform;
	
	/* Context. */
	cl_context context;
	
	/* Number of devices in context. */
	cl_uint num_devices;
	
	/* Devices in context. */
	CL4Device** devices;
	
};

CL4Context* cl4_context_new(cl_uint num_devices, 
	const cl_device_id *devices, GError **err) {

	/* Make sure number of devices is not zero. */
	g_return_val_if_fail(num_devices > 0, NULL);
	
	/* Make sure device list is not NULL. */
	g_return_val_if_fail(devices != NULL, NULL);
	
	/* Make sure err is NULL or it is not set. */
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);

	/* The OpenCL scene to create. */
	CL4Context* ctx = NULL;
	
	/* Error reporting object. */
	GError* err_internal = NULL;
	
	/* Return status of OpenCL function calls. */
	cl_int ocl_status;
	
	/* OpenCL platform ID. */
	cl_platform_id platform = NULL;
	
	/* Context properties. */
	cl_context_properties ctx_props[3] = {CL_CONTEXT_PLATFORM, 0, 0};
	
	/* Create ctx. */
	ctx = g_slice_new0(CL4Context);
	
	/* Set number of devices. */
	ctx->num_devices = num_devices;
	
	/* Allocate space for device wrappers. */
	ctx->devices = g_slice_alloc0(num_devices * sizeof(CL4Device*));
	
	/* Add device wrappers to list of device wrappers. */
	for (guint i = 0; i < num_devices; i++) {

		/* Make sure devices in list are not NULL. */
		g_return_val_if_fail(devices[i] != NULL, NULL);

		/* Create new device wrapper, add it to list. */
		ctx->devices[i] = cl4_device_new(devices[i]);

	}
		
	/* Get context platform using first device. */
	platform = *((cl_platform_id*) cl4_device_info_value(
		ctx->devices[0], CL_DEVICE_PLATFORM, &err_internal));
	gef_if_err_propagate_goto(err, err_internal, error_handler);
	
	/* Create a platform wrapper object and keep it. */
	ctx->platform = cl4_platform_new(platform);

	/* Set platform ID in context properties. */
	ctx_props[1] = (cl_context_properties) platform;
	
	/* Create OpenCL context. */
	ctx->context = clCreateContext(ctx_props, num_devices, devices, NULL,
		NULL, &ocl_status);
	gef_if_error_create_goto(*err, CL4_ERROR, CL_SUCCESS != ocl_status, 
		CL4_OCL_ERROR, error_handler, 
		"Function '%s': unable to create cl_context (OpenCL error %d: %s).", 
		__func__, ocl_status, cl4_err(ocl_status));
	
	/* If we got here, everything is OK. */
	g_assert (err == NULL || *err == NULL);
	goto finish;
	
error_handler:
	/* If we got here there was an error, verify that it is so. */
	g_assert (err == NULL || *err != NULL);

	/* Destroy the ctx, or what was possible to build of it. */
	if (ctx != NULL)
		cl4_context_destroy(ctx);
	ctx = NULL;

finish:	

	/* Return ctx. */
	return ctx;
	
}


void cl4_context_destroy(CL4Context* ctx) {
	
	/* Make sure context wrapper object is not NULL. */
	g_return_if_fail(ctx != NULL);

	/* Release devices in ctx. */
	for (guint i = 0; i < ctx->num_devices; i++) {
		cl4_device_unref(ctx->devices[i]);
	}
	
	/* Free device array. */
	g_slice_free1(
		ctx->num_devices * sizeof(CL4Device*), ctx->devices);
              
	/* Release context. */
	if (ctx->context) {
		clReleaseContext(ctx->context);
	}
	
	/* Release platform. */
	if (ctx->platform) {
		cl4_platform_unref(ctx->platform);
	}
	
	/* Release ctx. */
	g_slice_free(CL4Context, ctx);

}
