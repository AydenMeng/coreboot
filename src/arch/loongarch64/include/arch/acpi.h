/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __ARCH_ACPI_H_
#define __ARCH_ACPI_H_

/* For LoongArch64, ACPI wakeup is not supported (always cold boot). */
static inline int acpi_is_wakeup(void) { return 0; }
static inline int acpi_is_wakeup_s3(void) { return 0; }
static inline int acpi_is_wakeup_s4(void) { return 0; }

/* MADT interrupt controller types for LoongArch (ACPI 6.5+). */
#define ACPI_MADT_TYPE_CORE_PIC		0x11
#define ACPI_MADT_TYPE_LIO_PIC		0x12
#define ACPI_MADT_TYPE_EIO_PIC		0x13
#define ACPI_MADT_TYPE_MSI_PIC		0x14
#define ACPI_MADT_TYPE_BIO_PIC		0x15
#define ACPI_MADT_TYPE_HTPIC		0x16

#endif /* __ARCH_ACPI_H_ */
