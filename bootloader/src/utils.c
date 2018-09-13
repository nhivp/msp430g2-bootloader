/*
 * Created by nhivp on 19/08/2018.
 * Copyright (C) 2018 nhivp. All rights reserved.
 */

#include "utils.h"

static unsigned char asciiToHex(char ascii);
static unsigned int hexToBin(const char *hex);

/*
 * A record in intel hex looks like
 * :LLXXXXTTDDDD....DDCC
 */
unsigned char hexStringToBin(const char *str, ihex_format_t *hex)
{
    unsigned char result;
    unsigned int offset;
    unsigned int index;
    unsigned char checkSum;
    unsigned int temp;

    /* start */
    if(str[0] != ':')
    {
        result = RESPONE_FAIL;
    }

    offset = 0;
    checkSum = 0;
    /* length */
    offset ++;
    hex->length = hexToBin(str + offset);
    checkSum += hex->length;

    /* A record with 16-byte length data */
    if(hex->length > 16)
    {
        result = RESPONE_FAIL;
    }

    /* 2-byte address */
    hex->address = 0;
    for(index = 0;index < 2;index ++)
    {
        offset += 2;
        temp = hexToBin(str + offset);
        checkSum += temp;
        hex->address |= temp << 8 * (1 - index);
    }

    /* 1-byte type */
    offset += 2;
    hex->type = hexToBin(str + offset);
    checkSum += hex->type;

    /* data */
    for(index = 0;index < hex->length ;index ++)
    {
        offset += 2;
        temp = hexToBin(str + offset);
        hex->data[index] = temp;
        checkSum += temp;
    }

    /* checksum */
    offset += 2;
    temp = hexToBin(str + offset);
    if(temp == (unsigned int)(0x100 - checkSum)) /* CRC = The two's complement of 'checkSum' */
    {
        result = RESPONE_SUCCESS;
    }
    else
    {
        result = RESPONE_FAIL;
    }

    return result;
}

static unsigned char asciiToHex(char ascii)
{
    unsigned char result;

    if(ascii <= '9' && ascii >= '0')
    {
        result = ascii - '0';
    }
    else if(ascii >= 'a' && ascii <= 'f')
    {
        result = ascii - 'a' + 10;
    }
    else if(ascii >= 'A' && ascii <= 'F')
    {
        result = ascii - 'A' + 10;
    }
    else
    {
        result = 0xFF;
    }

    return result;
}

static unsigned int hexToBin(const char *hex)
{
    unsigned int temp;

    temp = asciiToHex(hex[0]) << 4;
    temp |= asciiToHex(hex[1]);

    return temp;
}
