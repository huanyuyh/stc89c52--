#ifndef __UART_H__
#define __UART_H__

void UART_Init_12M();
void UART_Init_110592();
void UART_SendChar(char Byte);
void UART_SendString(char* str);
void UART_SendNum(int num);
void UART_SendOne(int num);
#endif
