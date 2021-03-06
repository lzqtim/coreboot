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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <arch/cache.h>
#include <arch/exception.h>
#include <arch/hlt.h>
#include <arch/stages.h>
#include <console/console.h>
#include <soc/verstage.h>
#include <vendorcode/google/chromeos/chromeos.h>

void __attribute__((weak)) verstage_mainboard_init(void)
{
	/* Default empty implementation. */
}

static void verstage(void)
{
	void *entry;

	console_init();
	exception_init();
	verstage_mainboard_init();

	entry = vboot2_verify_firmware();
	if (entry != (void *)-1)
		stage_exit(entry);
}

void main(void)
{
	verstage();
	hlt();
}
