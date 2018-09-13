/*
 * Created by nhivp on 19/08/2018.
 * Copyright (C) 2018 nhivp. All rights reserved.
 */

#ifndef _UART_H_
#define _UART_H_

void UART_Initialize(void);
void UART_SendChar(char ch);
void UART_ReadChar(char *ch);
void UART_SendString(char *str);
void UART_ReadString(char *str,unsigned int num);

#endif /* _UART_H_ */