/* SPDX-License-Identifier: GPL-2.0-only */
#include <boot_device.h>
#include <symbols.h>
#include <mainboard/addressmap.h>

static const struct mem_region_device boot_dev =
	MEM_REGION_DEV_RO_INIT(0x1c000000, 0x400000);

const struct region_device *boot_device_ro(void)
{
	return &boot_dev.rdev;
}
