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
	//
	// Declare the local variables that actually move the data elements as
	// volatile to prevent the optimizer from replacing this function with
	// the intrinsic memcpy()
	//
	volatile uint8_t   *dst8;
	const uint8_t      *src8;
	volatile uint32_t  *dst32;
	const uint32_t     *src32;
	volatile uint64_t  *dst64;
	const uint64_t     *src64;
	uint64_t            alian;

	if ((((uint64_t)dst_buffer & 0x7) == 0) && (((uint64_t)src_buffer & 0x7) == 0) && (len >= 8)) {
		if (src_buffer > dst_buffer) {
			dst64 = (uint64_t *)dst_buffer;
			src64      = (const uint64_t *)src_buffer;
			while (len >= 8) {
				*(dst64++) = *(src64++);
				len            -= 8;
			}

			// Finish if there are still some bytes to copy
			dst8 = (uint8_t *)dst64;
			src8      = (const uint8_t *)src64;
			while (len-- != 0) {
				*(dst8++) = *(src8++);
			}
		} else if (src_buffer < dst_buffer) {
			dst64 = (uint64_t *)((uint64_t)dst_buffer + len);
			src64      = (const uint64_t *)((uint64_t)src_buffer + len);

			// dst64 and src64 were aligned on a 64-bit boundary
			// but if length is not a multiple of 8 bytes then they won't be
			// anymore.

			alian = len & 0x7;
			if (alian != 0) {
				dst8 = (uint8_t *)dst64;
				src8      = (const uint8_t *)src64;

				while (alian-- != 0) {
					*(--dst8) = *(--src8);
					--len;
				}

				dst64 = (uint64_t *)dst8;
				src64      = (const uint64_t *)src8;
			}

			while (len > 0) {
				*(--dst64) = *(--src64);
				len            -= 8;
			}
		}
	} else if ((((uint64_t)dst_buffer & 0x3) == 0) && (((uint64_t)src_buffer & 0x3) == 0) && (len >= 4)) {
		if (src_buffer > dst_buffer) {
			dst32 = (uint32_t *)dst_buffer;
			src32      = (const uint32_t *)src_buffer;
			while (len >= 4) {
				*(dst32++) = *(src32++);
				len            -= 4;
			}

			// Finish if there are still some bytes to copy
			dst8 = (uint8_t *)dst32;
			src8      = (const uint8_t *)src32;
			while (len-- != 0) {
				*(dst8++) = *(src8++);
			}
		} else if (src_buffer < dst_buffer) {
			dst32 = (uint32_t *)((uint64_t)dst_buffer + len);
			src32      = (const uint32_t *)((uint64_t)src_buffer + len);

			// dst32 and src32 were aligned on a 32-bit boundary
			// but if length is not a multiple of 4 bytes then they won't be
			// anymore.

			alian = len & 0x3;
			if (alian != 0) {
				dst8 = (uint8_t *)dst32;
				src8      = (const uint8_t *)src32;

				while (alian-- != 0) {
					*(--dst8) = *(--src8);
					--len;
				}

				dst32 = (uint32_t *)dst8;
				src32      = (const uint32_t *)src8;
			}

			while (len > 0) {
				*(--dst32) = *(--src32);
				len            -= 4;
			}
		}
	} else {
		if (src_buffer > dst_buffer) {
			dst8 = (uint8_t *)dst_buffer;
			src8      = (const uint8_t *)src_buffer;
			while (len-- != 0) {
				*(dst8++) = *(src8++);
			}
		} else if (src_buffer < dst_buffer) {
			dst8 = (uint8_t *)dst_buffer + (len - 1);
			src8      = (const uint8_t *)src_buffer + (len - 1);
			while (len-- != 0) {
				*(dst8--) = *(src8--);
			}
		}
	}

	return dst_buffer;
}
