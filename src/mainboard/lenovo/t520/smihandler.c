/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2008-2009 coresystems GmbH
 * Copyright (C) 2014 Vladimir Serbinenko
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <arch/io.h>
#include <console/console.h>
#include <cpu/x86/smm.h>
#include <ec/acpi/ec.h>
#include <pc80/mc146818rtc.h>
#include <ec/lenovo/h8/h8.h>
#include <delay.h>
#include <southbridge/intel/bd82x6x/nvs.h>
#include <southbridge/intel/bd82x6x/pch.h>
#include <southbridge/intel/bd82x6x/me.h>
#include <northbridge/intel/sandybridge/sandybridge.h>
#include <cpu/intel/model_206ax/model_206ax.h>

#define GPE_EC_SCI	1
#define GPE_EC_WAKE	13

/* The southbridge SMI handler checks whether gnvs has a
 * valid pointer before calling the trap handler
 */
extern global_nvs_t *gnvs;

static void mainboard_smm_init(void)
{
	printk(BIOS_DEBUG, "initializing SMI\n");
	/* Enable 0x1600/0x1600 register pair */
	ec_set_bit(0x00, 0x05);
}

int mainboard_io_trap_handler(int smif)
{
	static int smm_initialized;

	if (!smm_initialized) {
		mainboard_smm_init();
		smm_initialized = 1;
	}

	/* On success, the IO Trap Handler returns 1
	 * On failure, the IO Trap Handler returns a value != 1 */
	return 0;
}

static void mainboard_smi_brightness_up(void)
{
	u8 value;

	if ((value = pci_read_config8(PCI_DEV(0, 2, 1), 0xf4)) < 0xf0)
		pci_write_config8(PCI_DEV(0, 2, 1), 0xf4, (value + 0x10) | 0xf);
}

static void mainboard_smi_brightness_down(void)
{
	u8 value;

	if ((value = pci_read_config8(PCI_DEV(0, 2, 1), 0xf4)) > 0x10)
		pci_write_config8(PCI_DEV(0, 2, 1), 0xf4,
		(value - 0x10) & 0xf0);
}

static void mainboard_smi_handle_ec_sci(void)
{
	u8 status = inb(EC_SC);
	u8 event;

	if (!(status & EC_SCI_EVT))
		return;

	event = ec_query();
	printk(BIOS_DEBUG, "EC event %02x\n", event);

	switch (event) {
	case 0x14:
		/* brightness up */
		mainboard_smi_brightness_up();
		break;
	case 0x15:
		/* brightness down */
		mainboard_smi_brightness_down();
		break;
	default:
		break;
	}
}

void mainboard_smi_gpi(u32 gpi_sts)
{
	if (gpi_sts & (1 << GPE_EC_SCI))
		mainboard_smi_handle_ec_sci();
}

static int mainboard_finalized = 0;

int mainboard_smi_apmc(u8 data)
{
	switch (data) {
	case APM_CNT_ACPI_ENABLE:
		/* use 0x1600/0x1604 to prevent races with userspace */
		ec_set_ports(0x1604, 0x1600);
		/* route EC_SCI to SCI */
		gpi_route_interrupt(GPE_EC_SCI, GPI_IS_SCI);
		/* discard all events, and enable attention */
		ec_write(0x80, 0x01);
		break;
	case APM_CNT_ACPI_DISABLE:
		/* we have to use port 0x62/0x66, as 0x1600/0x1604 doesn't
		   provide a EC query function */
		ec_set_ports(0x66, 0x62);
		/* route EC_SCI to SMI */
		gpi_route_interrupt(GPE_EC_SCI, GPI_IS_SMI);
		/* discard all events, and enable attention */
		ec_write(0x80, 0x01);
		break;
	case APM_CNT_FINALIZE:
		printk(BIOS_DEBUG, "APMC: FINALIZE\n");
		if (mainboard_finalized) {
			printk(BIOS_DEBUG, "APMC#: Already finalized\n");
			return 0;
		}

		intel_me_finalize_smm();
		intel_pch_finalize_smm();
		intel_sandybridge_finalize_smm();
		intel_model_206ax_finalize_smm();

		mainboard_finalized = 1;
		break;

	default:
		break;
	}
	return 0;
}

void mainboard_smi_sleep(u8 slp_typ)
{
	if (slp_typ == 3) {
		u8 ec_wake = ec_read(0x32);
		/* If EC wake events are enabled, enable wake on EC WAKE GPE.  */
		if (ec_wake & 0x14) {
			/* Redirect EC WAKE GPE to SCI.  */
			gpi_route_interrupt(GPE_EC_WAKE, GPI_IS_SCI);
		}
	}
}
