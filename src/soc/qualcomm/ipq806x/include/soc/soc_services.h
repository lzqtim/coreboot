/*
 * This file is part of the coreboot project.
 *
 * Copyright 2014 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __SOC_QUALCOMM_IPQ806X_INCLUDE_SOC_SOC_SERVICES_H__
#define __SOC_QUALCOMM_IPQ806X_INCLUDE_SOC_SOC_SERVICES_H__

#include <types.h>

extern u8 _memlayout_cbmem_top[];

/* Returns zero on success, nonzero on failure. */
int initialize_dram(void);

/* Loads and runs TZBSP, switches into user mode. */
void start_tzbsp(void);

/* A helper function needed to start TZBSP properly. */
int tz_init_wrapper(int, int, void *);

/* Load RPM code into memory and trigger its execution. */
void start_rpm(void);

#endif
