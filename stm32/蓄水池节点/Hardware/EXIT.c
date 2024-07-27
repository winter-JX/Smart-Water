#include "EXIT.h"
#include "LED.h"
#include "OLED.h"

extern uint8_t mode;
extern uint8_t mode_ex;

//三个按键PA0|PA15|PC5
void ExitKey_Config(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		//开启GPIO AFIO的时钟控制外设
		//EXIT和NVIC（内核外设）都不需要开启外设
		
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;  //上拉输入
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15 ;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//默认即可
		GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;  //XIA拉输入
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0 ;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//默认即可
		GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;  //上拉输入
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5 ;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//默认即可
		GPIO_Init(GPIOC,&GPIO_InitStruct);
		//配置GPIO端口
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
		//配置AFIO外设中断引脚外设（注：AFIO外设文件在GPIO文件里）
		
		EXTI_InitTypeDef EXTI_InitStruct;
		EXTI_InitStruct.EXTI_Line=EXTI_Line15 | EXTI_Line0 | EXTI_Line5;//选择中断线
		EXTI_InitStruct.EXTI_LineCmd=ENABLE;//选择EXTI的状态，使能/失能
		EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//选择EXTI的模式，中断响应/事件响应
		EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//指定EXTI线的触发信号激活边缘，上升沿/下降沿/双边沿（注，库函数中，后面的应该是EXTITrigger_TypeDef）
		EXTI_Init(&EXTI_InitStruct);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;//指定要启用或禁用的IRQ通道
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能或失能
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//指定NVIC_IRQChannel中指定的IRQ通道的抢占优先级
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;//指定NVIC_IRQChannel中指定的IRQ通道的响应优先级
		NVIC_Init(&NVIC_InitStruct);
		
		NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;//指定要启用或禁用的IRQ通道
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能或失能
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//指定NVIC_IRQChannel中指定的IRQ通道的抢占优先级
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//指定NVIC_IRQChannel中指定的IRQ通道的响应优先级
		NVIC_Init(&NVIC_InitStruct);
		
		NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//指定要启用或禁用的IRQ通道
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能或失能
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//指定NVIC_IRQChannel中指定的IRQ通道的抢占优先级
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//指定NVIC_IRQChannel中指定的IRQ通道的响应优先级
		NVIC_Init(&NVIC_InitStruct);
}
  
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		LED2_Turn();
		mode_ex = 1;
		
		EXTI_ClearITPendingBit(EXTI_Line0); //清除Line0的中断标志位
  }
}


void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
  {
		LED1_Turn();
		mode += 1;
		mode = mode % 8;
		OLED_ShowNum(95,15,mode,1,16,1);
		OLED_Refresh();
		
		EXTI_ClearITPendingBit(EXTI_Line5); //清除Line0的中断标志位
  }
}
