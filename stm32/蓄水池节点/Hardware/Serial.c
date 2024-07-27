#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

char Serial_RxPacket[100];				//"@MSG\r\n"
uint8_t Serial_RxFlag;

char Serial_RxPacket2[100];				//"@MSG\r\n"
uint8_t Serial_RxFlag2;

char Serial_RxPacket3[100];				//"@MSG\r\n"
uint8_t Serial_RxFlag3;

char Serial_RxPacket4[100];				//"@MSG\r\n"
uint8_t Serial_RxFlag4;

void Serial_Init(void)
{
	//USART1  ----------------------------------------------------------------------------------------
	// LoRa
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
	
	//USART2 ----------------------------------------------------------------------------------------
	//水位传感器
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOC和USART4时钟
	
	//UART4_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//UART4_RX      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	//Uart4 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器
	
	//UART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口3
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  //开启串口接收中断
	USART_Cmd(USART2, ENABLE);                    //使能串口3 
	
	//USART3  ----------------------------------------------------------------------------------------
	//上位机
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启GPIOA和USART1时钟
	
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.10
	
	//USART3_RX      GPIOB.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11  
	
	//Usart3 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);  //开启串口接收中断
	USART_Cmd(USART3, ENABLE);                    //使能串口3 
	
	
//	//UART4 注意 没有S ----------------------------------------------------------------------------------------
//	//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//开启GPIOC和USART4时钟
//	
//	//UART4_TX   GPIOC.10
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC.10
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
//	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.10
//	
//	//UART4_RX      
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PC11
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.11  
//	
//	//Uart4 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器
//	
//	//UART 初始化设置
//	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
//	USART_Init(UART4, &USART_InitStructure); //初始化串口3
//	
//	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);  //开启串口接收中断
//	USART_Cmd(UART4, ENABLE);                    //使能串口3 

}

void Serial_SendByte(USART_TypeDef* USARTx, uint8_t Byte)
{
    // 发送数据到指定的USART
    USART_SendData(USARTx, Byte);
    
    // 等待发送完成
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(USART_TypeDef* USARTx, uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(USARTx, Array[i]);
	}
}

void Serial_SendString(USART_TypeDef* USARTx, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(USARTx, String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(USART_TypeDef* USARTx, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(USARTx, Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

//int fputc(int ch, FILE *f)
//{
//	Serial_SendByte(USARTx, ch);
//	return ch;
//}

void Serial_Printf(USART_TypeDef* USARTx, char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(USARTx, String);
}

void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
		if (RxState == 0)
		{
			if (RxData == '!' && Serial_RxFlag == 0)
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)
		{
			if (RxData == '\r')
			{
				RxState = 2;
			}
			else
			{
				Serial_RxPacket[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
		else if (RxState == 2)
		{
			if (RxData == '\n')
			{
				RxState = 0;
				Serial_RxPacket[pRxPacket] = '\0';
				Serial_RxFlag = 1;
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART2);
			
		if (RxState == 0)
		{
			if (RxData == '#' && Serial_RxFlag2 == 0)
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)
		{
			if (RxData == '\r')
			{
				RxState = 2;
			}
			else
			{
				Serial_RxPacket2[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
		else if (RxState == 2)
		{
			if (RxData == '\n')
			{
				RxState = 0;
				Serial_RxPacket2[pRxPacket] = '\0';
				Serial_RxFlag2 = 1;
			}
		}
		
	
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

void USART3_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint8_t pRxPacket = 0;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART3);
		
		if (RxState == 0)
		{
			if (RxData == '!' && Serial_RxFlag3 == 0)
			{
				RxState = 1;
				pRxPacket = 0;
			}
		}
		else if (RxState == 1)
		{
			if (RxData == '\r')
			{
				RxState = 2;
			}
			else
			{
				Serial_RxPacket3[pRxPacket] = RxData;
				pRxPacket ++;
			}
		}
		else if (RxState == 2)
		{
			if (RxData == '\n')
			{
				RxState = 0;
				Serial_RxPacket3[pRxPacket] = '\0';
				Serial_RxFlag3 = 1;
			}
		}
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
   }
}



//void UART4_IRQHandler(void)
//{
//    static uint8_t RxState = 0;
//    static uint8_t pRxPacket = 0;
//    if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
//	{
//		uint8_t RxData = USART_ReceiveData(UART4);
//		
//		if (RxState == 0)
//		{
//			if (RxData == '#' && Serial_RxFlag4 == 0)
//			{
//				RxState = 1;
//				pRxPacket = 0;
//			}
//		}
//		else if (RxState > 0 && RxState < 5)
//		{
//				Serial_RxPacket4[pRxPacket] = RxData;
//				pRxPacket ++;
//			}
//		}
//		else if (RxState == 5)
//		{
//			RxState = 0;
//			Serial_RxPacket4[pRxPacket] = '\0';
//			Serial_RxFlag4 = 1;
//		}
//    
//		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
//}

