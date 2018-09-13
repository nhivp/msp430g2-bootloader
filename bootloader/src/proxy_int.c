/*
 * Created by nhivp on 19/08/2018.
 * Copyright (C) 2018 nhivp. All rights reserved.
 */

#include "msp430g2553.h"

extern unsigned int __attribute__((section(".mode_flag"))) isBootMode;
extern unsigned int count;

/*
 * It mirrors the interrupt vector table into the "application" space at a new segment.
 * The bootloader uses the actual vectors and remaps them to the application space.
 */

void __attribute((interrupt(1))) TRAPINT_Handler()
{
    asm("br &0xCA00");
}

void __attribute((interrupt(3))) PORT1_Handler()
{
    asm("br &0xCA04");
}

void __attribute((interrupt(4))) PORT2_Handler()
{
    asm("br &0xCA06");
}

void __attribute((interrupt(6))) ADC10_Handler()
{
    asm("br &0xCA0A");
}

void __attribute((interrupt(7))) USCIAB0TX_Handler()
{
    asm("br &0xCA0C");
}

void __attribute((interrupt(8))) USCIAB0RX_Handler()
{
    asm("br &0xCA0E");
}

void __attribute((interrupt(9))) TIMER0_A1_Handler()
{
    asm("br &0xCA10");
}

void __attribute((interrupt(10))) TIMER0_A0_Handler()
{
    if(isBootMode)
    {
        count ++;
    }
    else
    {
        asm("br &0xCA12");
    }
}

void __attribute((interrupt(11))) WDT_Handler()
{
    asm("br &0xCA14");
}

void __attribute((interrupt(12))) COMPARATORA_Handler()
{
    asm("br &0xCA16");
}

void __attribute((interrupt(13))) TIMER1_A1_Handler()
{
    asm("br &0xCA18");
}

void __attribute((interrupt(14))) TIMER1_A0_Handler()
{
    asm("br &0xCA1A");
}

void __attribute((interrupt(15))) NMI_Handler()
{
    asm("br &0xCA1C");
}
