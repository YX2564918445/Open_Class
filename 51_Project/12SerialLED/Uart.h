#ifndef __UART_H
#define __UART_H

/* 函数声明 */
void Uart_Init(void);
void Uart_SendByte(unsigned char dat);
void Uart_SendString(unsigned char *str);
unsigned char Uart_GetCommand(unsigned char *cmd);
void Uart_CheckIdle(void);

#endif