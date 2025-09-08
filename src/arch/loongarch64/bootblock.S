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
#define bootcore_id  0

.globl _start
_start:
  /* disable global interrupt */
  bl       disable_interrupts

  /* disable all local interrupt */
  li.w     $a0, 0x1fff
  bl       disable_local_interrupts

  /* read physical cpu number id */
  bl       get_apic_id
  li.d     $t0, bootcore_id  //0
  bne      $a0, $t0, slave_main

  /* set bsp stack */
  li.d     $t0, SEC_PEI_TEMP_RAM_BASE + SEC_PEI_TEMP_RAM_SIZE  # stack base
  move     $sp, $t0
  addi.d   $sp, $sp, -0x8

  /* load the exception vector base address */
  li.d     $s0, LOONGARCH_EXCEPTION_VECTOR_BASE_ADDRESS

  /* construct sec and pei step exception environment */
  la.pcrel $a1, exception_entry_start
  la.pcrel $t0, exception_entry_end
  sub.d    $a2, $t0, $a1
  li.w     $t0, (MAX_LOONGARCH_EXCEPTION + MAX_LOONGARCH_INTERRUPT) * 512
  bgeu     $a2, $t0, dead_loop
  move     $a0, $s0
  bl       copy_mem

  /* configure bsp reset ebase */
  move     $a0, $s0
  bl       set_exception_base_address

call_entry:
  /* call c function make sure parameter true */
  li.d     $a0, FW_BASE_ADDRESS # fw base
  addi.d   $a1, $sp, 0x8
  //bl       sec_core_startup_with_stack //MXD-TODO: main.c
  bl       main
# end of _module_entry_point

clear_mail_box:
  /* clear mailbox */
  li.d      $t1, LOONGARCH_IOCSR_MBUF3
  iocsrwr.d $zero, $t1
  li.d      $t1, LOONGARCH_IOCSR_MBUF2
  iocsrwr.d $zero, $t1
  li.d      $t1, LOONGARCH_IOCSR_MBUF1
  iocsrwr.d $zero, $t1
  li.d      $t1, LOONGARCH_IOCSR_MBUF0
  iocsrwr.d $zero, $t1
  jirl      $zero, $ra, 0
# end of clear_mail_box

enable_ipi:
  /* enable ipi interrupt */
  li.w      $t1, BIT12
  csrxchg   $t1, $t1, LOONGARCH_CSR_ECFG

  li.w      $t2, 0xffffffff
  li.d      $t1, LOONGARCH_IOCSR_IPI_EN
  iocsrwr.w $t2, $t1
  jirl      $zero, $ra, 0
# end of eeable_ipi

#/**
#   get apic id for every cpu.
#
#   @param   null
#   @return  apicid
#
#   uintn
#   efi_api
#   get_apic_id (
#     void
#     )
#**/
get_apic_id:
  csrrd $a0, LOONGARCH_CSR_CPUID
  andi  $a0, $a0, 0x3ff
  jirl  $zero, $ra, 0
# end of get_apic_id

ap_init_stack:
  li.d   $t1, SIZE_1KB
  csrrd  $t0, LOONGARCH_CSR_TMID
  mul.d  $t1, $t0, $t1
  li.d   $t2, CPU_MAX_LOGICAL_PROCESSOR_NUMBER
  bgeu   $t0, $t2, dead_loop
  li.d   $t0, SEC_PEI_TEMP_RAM_BASE + SEC_PEI_TEMP_RAM_SIZE - SIZE_64KB
  sub.d  $sp, $t0, $t1
  addi.d $sp, $sp, -0x8
  jirl   $zero, $ra, 0
# end of ap_init_stack

slave_main:
  /* set ap exception handle in flash */
  la.pcrel  $a0, ap_exception
  bl        set_exception_base_address

  /* clean up local mail box and open int */
  bl        clear_mail_box
  bl        enable_ipi
  bl        enable_interrupts

wait_for_wake:
  /* wait for wakeup */
  bl        cpu_sleep
  b         wait_for_wake
# end of slave_main

.align 12
ap_exception:
  csrrd     $t0, LOONGARCH_CSR_ESTAT
  srli.d    $t0, $t0, 12
  andi      $t0, $t0, 0x1
  beqz      $t0, dead_loop

  li.d      $t0, LOONGARCH_IOCSR_IPI_STATUS
  iocsrrd.w $t1, $t0
  li.d      $t0, LOONGARCH_IOCSR_IPI_CLEAR
  iocsrwr.w $t1, $t0

  /* read mail buf and jump to specified entry */
  li.d      $t1, LOONGARCH_IOCSR_MBUF0
  iocsrrd.d $t0, $t1
  beqz      $t0, out_of_exception
  csrwr     $t0, LOONGARCH_CSR_ERA
  li.d      $t0, LOONGARCH_IOCSR_MBUF3
  iocsrrd.d $a1, $t0
  bl        clear_mail_box
  beqz      $a1, no_parameter_call

  //
  // if the parameters are not null, then calling happened in fw env.
  // set the ebase to be the same as bsp.
  //
  li.d      $a0, LOONGARCH_EXCEPTION_VECTOR_BASE_ADDRESS
  bl        set_exception_base_address

  bl        ap_init_stack
  bl        get_apic_id
  b         out_of_exception
no_parameter_call:
  li.w      $t0, BIT2 // ie
  csrxchg   $zero, $t0, LOONGARCH_CSR_PRMD // clean pie

out_of_exception:
  ertn
# end of ap_exception

cpu_sleep:
  idle 0
  jirl    $zero, $ra, 0
# end of dead_loop

dead_loop:
  b   dead_loop
# end of dead_loop
