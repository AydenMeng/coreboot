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
.global enable_floating_point_units
.global disable_floating_point_units

enable_floating_point_units:
  li.w        $t0, 0x1
  csrxchg     $t0, $t0, 0x2

  jirl $zero, $ra, 0

disable_floating_point_units:
  li.w        $t0, 0x1
  csrxchg     $zero, $t0, 0x2

  jirl $zero, $ra, 0
