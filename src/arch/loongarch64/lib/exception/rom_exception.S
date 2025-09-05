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

.global  set_exception_base_address
.global  set_tlb_rebase_address
#/**
#  set the exception base address for loong_arch.
#
#  @param  exception_base_address   the exception base address, must be aligned greater than or qeual to 4k .
#**/
set_exception_base_address:
  csrrd   $t0, LOONGARCH_CSR_ECFG
  li.d    $t1, ~(BIT16 | BIT17 | BIT18)
  and     $t0, $t0, $t1
  csrwr   $t0, LOONGARCH_CSR_ECFG

  move    $t0, $a0
  csrwr   $t0, LOONGARCH_CSR_EBASE
  jirl    $zero, $ra, 0

#/**
#  set the tlb_rebase address for loong_arch.
#
#  @param  tlb_rebase_address   the tlb_rebase address, must be aligned greater than or qeual to 4k .
#**/
set_tlb_rebase_address:
  move    $t0, $a0
  csrwr   $t0, LOONGARCH_CSR_TLBREBASE
  jirl    $zero, $ra, 0
