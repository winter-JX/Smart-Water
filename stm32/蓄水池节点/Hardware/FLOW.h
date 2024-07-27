#ifndef _FLOW_H
#define _FLOW_H

#include "stm32f10x.h"
//#include "usart.h"
#include "LED.h"

typedef struct
{
    uint8_t receive_flag;
    uint16_t pluse_1s;
    
    float instant;
    float acculat;
}GOLBAL_FLOW;

extern GOLBAL_FLOW  golbal_flow;

void Exit2_Config(void);
void Flow_Read(void);
void TIM3_Int_Init(u16 arr,u16 psc);

#endif
