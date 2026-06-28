/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * C-only helpers for LoongArch CSR register access.
 * This header must NOT be included from assembly files.
 *
 * All helpers use macros (not inline functions) because the LoongArch CSR
 * instructions encode the register number as an immediate in the instruction
 * word.  A function parameter cannot satisfy the "i" constraint even when
 * the call site passes a literal constant — the compiler may choose not to
 * inline, or it may not propagate the constant far enough for the back‑end.
 */

#ifndef __CSR_FUNC_H__
#define __CSR_FUNC_H__

#include <csr.h>
#include <stdint.h>

/* Read CSR register (CSR number must be a compile‑time constant). */
#define csr_read(addr)						\
	({							\
		uint64_t _v;					\
		__asm__ __volatile__(				\
			"csrrd %0, %1"				\
			: "=&r"(_v)				\
			: "i"(addr)				\
		);						\
		_v;						\
	})

/* Write CSR register (CSR number must be a compile‑time constant). */
#define csr_write(addr, val)					\
	__asm__ __volatile__(					\
		"csrwr %1, %0"					\
		:						\
		: "i"(addr), "r"((uint64_t)(val))		\
	)

/* Atomically modify CSR register bits (CSR number must be compile‑time). */
#define csr_xchg(addr, val, mask)				\
	__asm__ __volatile__(					\
		"csrxchg %1, %2, %0"				\
		:						\
		: "i"(addr), "r"((uint64_t)(val)),		\
		  "r"((uint64_t)(mask))				\
	)

#endif /* __CSR_FUNC_H__ */
