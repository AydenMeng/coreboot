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
#include <lib/exception.h>

void CommonExceptionHandler(int)
{

}

/**
  Get Exception Type

  @param[in] SystemContext  Pointer to EFI_SYSTEM_CONTEXT.

  @return    LoongArch64 exception type.

**/
int GetExceptionType (void)
{
  return 0;
}

/**
  Get Interrupt Type

  @param[in] SystemContext  Pointer to EFI_SYSTEM_CONTEXT.

  @return    LoongArch64 intrrupt type.

**/
int GetInterruptType (void)
{
  return 0xFF;
}

/**
  Display CPU information.

  @param ExceptionType  Exception type.
  @param SystemContext  Pointer to EFI_SYSTEM_CONTEXT.

**/
void DumpCpuContext (int)
{
}

/**
  Display CPU information.

  @param ExceptionType  Exception type.
  @param SystemContext  Pointer to EFI_SYSTEM_CONTEXT.

**/
void DumpImageAndCpuContent (int)
{
}

/**
  IPI Interrupt Handler.

  Generally, the IPI interrupt uses three vector:
  SMP_BOOT_CPU       BSP boots AP. The BSP may reside in OS or other non-UEFI environment.
  SMP_RESCHEDULE     BSP calls AP via UEFI MpInitLib.
  SMP_CALL_FUNCTION  BSP calls AP to jump specified pointer, which allows with one parameter.

  @param InterruptType    The type of interrupt that occurred
  @param SystemContext    A pointer to the system context when the interrupt occurred
**/
void IpiInterruptHandler (int)
{
}
