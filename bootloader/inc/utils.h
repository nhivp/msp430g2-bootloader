/*
 * Created by nhivp on 19/08/2018.
 * Copyright (C) 2018 nhivp. All rights reserved.
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#define RESPONE_SUCCESS     1
#define RESPONE_FAIL        0

typedef struct
{
    unsigned int length;
    unsigned int type;
    unsigned int address;
    unsigned char data[16];
} ihex_format_t;

extern unsigned char hexStringToBin(const char *str, ihex_format_t *bin);

#endif /* _UTILS_H_ */
