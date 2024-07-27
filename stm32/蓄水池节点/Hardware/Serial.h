#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
#include "stm32f10x.h"                  // Device header
#include <stdarg.h>


extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

extern char Serial_RxPacket2[];
extern uint8_t Serial_RxFlag2;

extern char Serial_RxPacket3[];
extern uint8_t Serial_RxFlag3;

extern char Serial_RxPacket4[];
extern uint8_t Serial_RxFlag4;

void Serial_Init(void);
void Serial_SendByte(USART_TypeDef* USARTx, uint8_t Byte);
void Serial_SendArray(USART_TypeDef* USARTx, uint8_t *Array, uint16_t Length);
void Serial_SendString(USART_TypeDef* USARTx, char *String);
void Serial_SendNumber(USART_TypeDef* USARTx, uint32_t Number, uint8_t Length);
void Serial_Printf(USART_TypeDef* USARTx, char *format, ...);

#endif
