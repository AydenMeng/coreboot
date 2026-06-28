/* SPDX-License-Identifier: GPL-2.0-only */
#include <boot_device.h>
#include <symbols.h>
#include <mainboard/addressmap.h>
#include <commonlib/region.h>
#include <string.h>

static ssize_t la_flash_readat(const struct region_device *rdev,
			       void *b, size_t offset, size_t size)
{
	memcpy(b, (void *)(QEMU_VIRT_FLASH + region_device_offset(rdev) + offset), size);
	return (ssize_t)size;
}

static void *la_flash_mmap(const struct region_device *rdev,
			   size_t offset, size_t unused)
{
	return (void *)(QEMU_VIRT_FLASH + region_device_offset(rdev) + offset);
}

static int la_flash_munmap(const struct region_device *rdev, void *mapping)
{
	(void)rdev; (void)mapping;
	return 0;
}

static const struct region_device_ops la_flash_ro_ops = {
	.readat = la_flash_readat,
	.mmap = la_flash_mmap,
	.munmap = la_flash_munmap,
};

static const struct region_device boot_dev =
	REGION_DEV_INIT(&la_flash_ro_ops, 0, 0x400000);

const struct region_device *boot_device_ro(void)
{
	return &boot_dev;
}
