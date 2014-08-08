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
 * Common cf4ocl definitions.
 * 
 * @author Nuno Fachada
 * @date 2014
 * @copyright [GNU Lesser General Public License version 3 (LGPLv3)](http://www.gnu.org/licenses/lgpl.html)
 * */
 
#ifndef _CCL_COMMON_H_
#define _CCL_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gprintf.h>
#include "oclversions.h"

#define CCL_VALIDFILECHARS "abcdefghijklmnopqrstuvwxyzABCDEFGH" \
	"IJKLMNOPQRSTUVWXYZ0123456789_."

/**
 * Error codes.
 * */
typedef enum ccl_error_code {
	/** Successful operation. */
	CCL_SUCCESS                = 0,
	/** Unable to open file. */
	CCL_ERROR_OPENFILE         = 1,
	/** Invalid program arguments. */
	CCL_ERROR_ARGS             = 2,
	/** Invalid data passed to a function. */
	CCL_ERROR_INVALID_DATA     = 3,
	/** Error writing to a stream. */
	CCL_ERROR_STREAM_WRITE     = 4,
	/** The requested OpenCL device was not found. */
	CCL_ERROR_DEVICE_NOT_FOUND = 5,
	/** The operation is not supported by the version of the
	 * selected OpenCL platform. */
	CCL_ERROR_UNSUPPORTED_OCL  = 6,
	/** Any other errors. */
	CCL_ERROR_OTHER            = 15
} CCLErrorCode;


/** 
 * If error is detected (`error_code != no_error_code`), 
 * create an error object (GError) and go to the specified label. 
 * 
 * @param[out] err GError* object.
 * @param[in] quark Quark indicating the error domain.
 * @param[in] error_condition Must result to true in order to create
 * error.
 * @param[in] error_code Error code to set.
 * @param[in] label Label to goto if error is detected.
 * @param[in] msg Error message in case of error.
 * @param[in] ... Extra parameters for error message. 
 * */
#define ccl_if_err_create_goto(err, quark, error_condition, error_code, label, msg, ...) \
	if (error_condition) { \
		g_set_error(&(err), (quark), (error_code), (msg), ##__VA_ARGS__); \
		goto label; \
	}

/** 
 * If error is detected in `err` object (`err != NULL`),
 * go to the specified label.
 * 
 * @param[in] err GError* object.
 * @param[in] label Label to goto if error is detected.
 * */
#define ccl_if_err_goto(err, label)	\
	if ((err) != NULL) { \
		goto label; \
	}

/** 
 * Same as ccl_if_err_goto(), but rethrows error in a source
 * GError to a new destination GError object.
 * 
 * @param[out] err_dest Destination GError** object.
 * @param[in] err_src Source GError* object.
 * @param[in] label Label to goto if error is detected.
 * */
#define ccl_if_err_propagate_goto(err_dest, err_src, label) \
	if ((err_src) != NULL) { \
		g_propagate_error(err_dest, err_src); \
		goto label; \
	}

/** Resolves to error category identifying string, in this case an error 
 * in the cf4ocl. */
#define CCL_ERROR ccl_error_quark()

/** Resolves to error category identifying string, in this case an error 
 * in the cf4ocl. */
#define CCL_OCL_ERROR ccl_ocl_error_quark()

/** Resolves to error category identifying string, in this case
 * an error in cf4ocl. */
GQuark ccl_error_quark(void);

/** Resolves to error category identifying string, in this case
 * an error in the OpenCL library. */
GQuark ccl_ocl_error_quark(void);

#endif
