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
 * @brief Functions for selection OpenCL devices and the associated 
 * context.
 *
 * @author Nuno Fachada
 * @date 2014
 * @copyright [GNU Lesser General Public License version 3 (LGPLv3)](http://www.gnu.org/licenses/lgpl.html)
 * */

#ifndef CL4_DEVSEL_H
#define CL4_DEVSEL_H

#if defined(__APPLE__) || defined(__MACOSX)
    #include <OpenCL/cl.h>
#else
    #include <CL/cl.h>
#endif
#include <glib.h>

typedef struct cl4_devsel_map CL4DevSelMap;

/**
 * @brief Abstract function for selecting OpenCL devices and the 
 * associated context based on the provided information.
 *
 * @param select_info Information required for selection (implementation
 * dependent).
 * @param err Error structure, to be populated if an error occurs.
 * @return An OpenCL context or NULL if no adequate context was found.
 */
typedef cl4_platform (*cl4_devsel)(void *select_info, GError **err);

/** @brief Implementation of ::cl4_devsel function which selects one or
 * more devices based on device information such as device name, device 
 * vendor and platform name. */
GHashTable* cl4_devsel_name(void *select_info, GError **err);

#endif
