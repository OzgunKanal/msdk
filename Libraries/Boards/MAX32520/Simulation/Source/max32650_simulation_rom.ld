/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

MEMORY {
    ROM        (rx) : ORIGIN = 0x00000000, LENGTH = 0x00020000 /* 128kB ROM */
    SPIX_FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 0x08000000 /* 128MB SPIX Flash */
    FLASH      (rx) : ORIGIN = 0x10000000, LENGTH = 0x00300000 /* 3MB Flash */
    SRAM0     (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000 /* 32kB SRAM0 */
    SRAM1     (rwx) : ORIGIN = 0x20008000, LENGTH = 0x00010000 /* 64kB SRAM1 */
    SRAM2     (rwx) : ORIGIN = 0x20018000, LENGTH = 0x00008000 /* 32kB SRAM2 */
    SRAM3     (rwx) : ORIGIN = 0x20020000, LENGTH = 0x00020000 /* 128kB SRAM3 */
    SRAM4     (rwx) : ORIGIN = 0x20040000, LENGTH = 0x00040000 /* 256kB SRAM4 */
    SRAM5     (rwx) : ORIGIN = 0x20080000, LENGTH = 0x00040000 /* 256kB SRAM5 */
    SRAM6     (rwx) : ORIGIN = 0x200C0000, LENGTH = 0x00040000 /* 256kB SRAM6 */
    HBUS      (rwx) : ORIGIN = 0x60000000, LENGTH = 0x20000000 /* 512MB Hyperbus */
    SPIX_DATA (rwx) : ORIGIN = 0x80000000, LENGTH = 0x20000000 /* 512MB SPIX Data */
}

SECTIONS {
    .rom :
    {
        KEEP(*(.rom_vector))
        *(.rom_handlers*)
    } > ROM

    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))
        *(.text*)    /* program code */
        *(.rodata*)  /* read-only data: "const" */

        KEEP(*(.init))
        KEEP(*(.fini))

        /* C++ Exception handling */
        KEEP(*(.eh_frame*))
        _etext = .;
    } > ROM

    .rom_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.rom_code_section)
        _esran_code = .;
    } > ROM

    .spix_flash_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.spix_flash_code_section)
        _esran_code = .;
    } > SPIX_FLASH

    .flash_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.flash_code_section)
        _esran_code = .;
    } > FLASH

    .sram0_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.sram0_code_section)
        _esran_code = .;
    } > SRAM0

    .sram1_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.sram1_code_section)
        _esran_code = .;
    } > SRAM1

    .sram2_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.sram2_code_section)
        _esran_code = .;
    } > SRAM2

    .sram3_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.sram3_code_section)
        _esran_code = .;
    } > SRAM3

    .sram4_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.sram4_code_section)
        _esran_code = .;
    } > SRAM4

    .sram5_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.sram5_code_section)
        _esran_code = .;
    } > SRAM5

    .sram6_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.sram6_code_section)
        _esran_code = .;
    } > SRAM6

    .hbus_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.hbus_code_section)
        _esran_code = .;
    } > HBUS

    .spix_data_code :
    {
        . = ALIGN(16);
        _sran_code = .;
        *(.spix_data_code_section)
        _esran_code = .;
    } > SPIX_DATA

    /* it's used for C++ exception handling      */
    /* we need to keep this to avoid overlapping */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } > ROM

    .data :
    {
        _data = ALIGN(., 4);
        *(.data*)           /*read-write initialized data: initialized global variable*/
        *(.spix_config*)    /* SPIX configuration functions need to be run from SRAM0 */

        /* These array sections are used by __libc_init_array to call static C++ constructors */
        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        _edata = ALIGN(., 4);
    } > SRAM0 AT>ROM
    __load_data = LOADADDR(.data);

    .bss :
    {
        . = ALIGN(4);
        _bss = .;
        *(.bss*)     /*read-write zero initialized data: uninitialzed global variable*/
        *(COMMON)
        _ebss = ALIGN(., 4);
    } > SRAM0

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(SRAM0) + LENGTH(SRAM0);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);

    /* .stack_dummy section doesn't contains any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy (COPY):
    {
        *(.stack*)
    } > SRAM0

    .heap (COPY):
    {
        . = ALIGN(4);
        *(.heap*)
        __HeapLimit = ABSOLUTE(__StackLimit);
    } > SRAM0

    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack exceeds RAM limit */
    ASSERT(__StackLimit >= _ebss, "region RAM overflowed with stack")
}
