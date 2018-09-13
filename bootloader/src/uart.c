/*
 * Created by nhivp on 19/08/2018.
 * Copyright (C) 2018 nhivp. All rights reserved.
 */

#include "uart.h"
#include "msp430g2553.h"

void UART_Initialize(void)
{
    /* Config Pin function */
    P1SEL = 0x06;
    P1SEL2 = 0x06;

    /* Config UART */
    UCA0CTL0 = 0x00;
    UCA0CTL1 = 0x01 << 7 | 0x01;
    UCA0BR0 = 104;
    UCA0BR1 = 0x00;
    UCA0MCTL = 0x01 << 1;
    UCA0CTL1 &= ~0x01;
}

void UART_SendChar(char ch)
{
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = ch;
    IFG2 &= ~UCA0TXIFG;
}

void UART_SendString(char *str)
{
    while (*str != '\0')
    {
        UART_SendChar(*str);
        str++;
    }
}

void UART_ReadChar(char *ch)
{
    while (!(IFG2 & UCA0RXIFG));
    *ch = UCA0RXBUF;
    IFG2 &= ~UCA0RXIFG;
}

void UART_ReadString(char *str, unsigned int length)
{
    unsigned int index;
    char temp;

    index = 0;
    temp = 0;

    while (1)
    {
        UART_ReadChar(&temp);
        UART_SendChar(temp);

        if (temp == '\r')
        {
            UART_SendChar('\n');
            break;
        }

        if (index < length)
        {
            str[index] = temp;
            index++;
        }
    }

    str[index] = '\0';
}
