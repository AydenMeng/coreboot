/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpi.h>
#include <device/mmio.h>

/*
 * Fill the FADT for the QEMU loongarch64 virt machine.
 *
 * The machine uses Hardware-Reduced ACPI.  Reset is provided
 * by the ACPI GED device at VIRT_GED_REG_ADDR (0x100e0020).
 */
void arch_fill_fadt(acpi_fadt_t *fadt)
{
	fadt->flags |= ACPI_FADT_HW_REDUCED_ACPI;

	/*
	 * RESET_REG: ACPI GED register at 0x100e0020
	 * address_space_id: 0 (System Memory)
	 * register_bit_width: 8
	 * register_bit_offset: 0
	 * access_size: 1 (byte access)
	 */
	fadt->reset_reg.space_id = ACPI_ADDRESS_SPACE_MEMORY;
	fadt->reset_reg.bit_width = 8;
	fadt->reset_reg.bit_offset = 0;
	fadt->reset_reg.access_size = 1;
	fadt->reset_reg.addrl = 0x100e0020;
	fadt->reset_reg.addrh = 0;
	fadt->reset_value = 0x1;
}
