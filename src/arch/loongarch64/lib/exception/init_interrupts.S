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

#include <csr.h>
#include <machine.h>
#include <loongarch64.h>
#include <c_quick.h>

.global enable_local_interrupts
.global enable_interrupts
.global disable_interrupts
.global disable_local_interrupts

#/**
#  disables global cpu interrupts.
#**/
disable_interrupts:
  li.w    $t0, BIT2
  csrxchg $zero, $t0, LOONGARCH_CSR_CRMD
  jirl    $zero, $ra, 0

#/**
#  disables local cpu interrupts.
#
#  @param  needs to disable local interrupt bit.
#**/
disable_local_interrupts:
  csrxchg $zero, $a0, LOONGARCH_CSR_ECFG
  jirl    $zero, $ra, 0
#/**
#  Enables local CPU interrupts.
#
#  @param  Needs to enable local interrupt bit.
#**/
enable_local_interrupts:
  csrxchg $a0, $a0, LOONGARCH_CSR_ECFG
  jirl    $zero, $ra, 0

#/**
#  Enables global CPU interrupts.
#**/
enable_interrupts:
  li.w    $t0, BIT2
  csrxchg $t0, $t0, LOONGARCH_CSR_CRMD
  jirl    $zero, $ra, 0
  .end
