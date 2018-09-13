/*
 * Created by nhivp on 19/08/2018.
 * Copyright (C) 2018 nhivp. All rights reserved.
 */

#include "flash.h"
#include "msp430g2553.h"

void FLASH_Initialize(void)
{
    FCTL2 = FWKEY + FSSEL_2 + FN3;
}

/* Segment size is 512B */
void FLASH_EraseSingleSegments(unsigned char * addr)
{
    unsigned char *ptr;

    ptr = addr;
    FCTL1 = FWKEY + ERASE;
    FCTL3 = FWKEY;

    *ptr = 0xFF;
    while (FCTL3 & BUSY);

    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}

void FLASH_WriteByte(unsigned char * addr, unsigned char data)
{
    unsigned char *ptr;

    ptr = addr;
    FCTL1 = FWKEY + WRT;
    FCTL3 = FWKEY;

    *ptr = data;
    while (FCTL3 & BUSY);

    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}