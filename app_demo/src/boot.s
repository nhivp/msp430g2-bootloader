    .global _start
    .global bootloader_main
    .global __datastart
    .global _rom_data_start
    .global _rom_data_end
    .global __bssstart
    .global __bssend
    .global __stack
    .set WDTCLOSE, (0x5A << 8) | 0x0080
    .set WDTOPEN,  (0x5A << 8) & 0xFF00

    .section .text
_start:
    mov.w #__stack, SP
    call #close_watch_dog
    call #copy_data
    call #clear_bss
    call #main

loop:
    jmp loop

close_watch_dog:
    mov.w #WDTCLOSE, &WDTCTL
    ret

copy_data:
    mov.w #_rom_data_start, R4
    mov.w #__datastart, R5
copy_data_loop:
    cmp.w #_rom_data_end, R4
    jeq copy_data_end
    mov @R4+, 0(R5)
    add #2, R5
    jmp copy_data_loop
copy_data_end:
    ret

clear_bss:
    mov.w #__bssstart, R4
clear_bss_loop:
    cmp.w #__bssend, R4
    jeq clear_bss_end
    mov.w #0x00, 0(R4)
    add.w #2, R4
    jmp clear_bss_loop
clear_bss_end:
    ret

    .section .resetvec, "a"
    .word _start
