/*
 * Copyright (c) 2025 Ayden Meng <aydenmeng@yeah.net>
 *
 * Portions Copyright (c) 2024 Loongson Technology Corporation Limited.
 *     Derived from EDK2 (https://github.com/tianocore/edk2)
 *     Original license: BSD-2-Clause-Patent
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * This file is part of coreboot.
 *
 * Description:
 *   Implements platform initialization based on EDK2 reference code.
 */


/**
  Copy len bytes from src to dst.

  @param  dst_buffer The target of the copy request.
  @param  src_buffer      The place to copy from.
  @param  len            The number of bytes to copy.

  @return dst

**/
#include <type.h>
#include <lib/mem_copy.h>
void *copy_mem (void *dst_buffer,const void *src_buffer, uint64_t len)
{
	    // 使用 uint8_t 指针进行逐字节拷贝
    uint8_t *d = (uint8_t *)dst_buffer;
    const uint8_t *s = (const uint8_t *)src_buffer;

    // 循环拷贝 len 个字节
    while (len > 0) {
        *d++ = *s++;
        len--;
    }

    // 返回原始目标指针
    return dst_buffer;
}
