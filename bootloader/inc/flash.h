/*
 * Created by nhivp on 19/08/2018.
 * Copyright (C) 2018 nhivp. All rights reserved.
 */

#ifndef _FLASH_H_
#define _FLASH_H_

void FLASH_Initialize(void);
void FLASH_EraseSingleSegments(unsigned char * addr);
void FLASH_WriteByte(unsigned char * addr, unsigned char data);

#endif /* _FLASH_H_ */