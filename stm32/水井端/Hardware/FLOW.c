#include "FLOW.h"
#include "OLED.h"

GOLBAL_FLOW golbal_flow;

#define Flow_Model_4       1            //4分管定义为1；6分管定义为0
#define MODE_4_K               62.0f
#define MODE_4_PLUSE_CNT_1L         3720.0f
#define MODE_6_K               5.5f
#define MODE_6_PLUSE_CNT_1L    330.0f
#define    FLOW_FULL                         1000000


//用PA7的外部中断来获取低电平的脉冲
void Exit2_Config(void)
{
//  EXTI_InitTypeDef EXTI_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 ;
//	
//	GPIO_Init(GPIOB,&GPIO_InitStruct);

//  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//  
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
//  
//  //Config EXTI2 line
//  EXTI_ClearITPendingBit(EXTI_Line2);
//  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //下降沿触发中断
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn; 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//  NVIC_Init(&NVIC_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		//开启GPIO AFIO的时钟控制外设
		//EXIT和NVIC（内核外设）都不需要开启外设
		
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//默认即可
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		//配置GPIO端口
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
		//配置AFIO外设中断引脚外设（注：AFIO外设文件在GPIO文件里）
		
		EXTI_InitTypeDef EXTI_InitStruct;
		EXTI_InitStruct.EXTI_Line=EXTI_Line14;//选择中断线
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
}
  
//void EXTI2_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
//  {
//    golbal_flow.pluse_1s++;           //下降沿采集脉冲数
//  }
//  EXTI_ClearITPendingBit(EXTI_Line2); //清除Line2的中断标志位
//}


void EXTI15_10_IRQHandler(void)//中断函数的格式
{
	if (EXTI_GetITStatus(EXTI_Line14)==SET)//判断是否为EXTI14进来，返回值为 SET or RESET
	{
		golbal_flow.pluse_1s++;
		EXTI_ClearITPendingBit(EXTI_Line14);//清除中断函数的标志位，否则程序将一直卡在中断函数中
	}
	
}

void Flow_Read(void)
{
		int g_flow = 0;  // 瞬时流量整数
	
    if(golbal_flow.receive_flag)
    {
        if(golbal_flow.pluse_1s > 0)
        {
      #ifdef Flow_Model_4
            /*计算公式：
                累计流量 = 对瞬时流量做积分
                                 = (脉冲频率 / 3720个脉冲)    //1L需要3720个脉冲，脉冲频率HZ
            */
            golbal_flow.acculat += golbal_flow.pluse_1s / MODE_4_PLUSE_CNT_1L * 1000;   //单位mL
                
            /*计算公式：
                        瞬时流量 = (脉冲频率 / 3720个脉冲) * 60s 
                                         = 脉冲频率 / 62.0(流量系K)
            */
            golbal_flow.instant = golbal_flow.pluse_1s / MODE_4_K * 1000;  //单位（mL/min）
      
      #else
          /*计算公式：
                累计流量 = 对瞬时流量做积分
                                 = (脉冲频率 / 330个脉冲)    //1L需要330个脉冲，脉冲频率HZ
            */
            golbal_flow.acculat += golbal_flow.pluse_1s / MODE_6_PLUSE_CNT_1L;   //单位L
                
            /*计算公式：
                        瞬时流量 = ((脉冲频率 + 3) / 330个脉冲) * 60s 
                                         = (脉冲频率 + 3) / 5.5(流量系K)
            */
            golbal_flow.instant = (golbal_flow.pluse_1s + 3) / MODE_6_K;  //单位（L/min）
      #endif
					g_flow = golbal_flow.acculat* 1000 ;
					OLED_ShowNum(40,15,g_flow,4,16,1);
					
      
      if(golbal_flow.acculat >= FLOW_FULL)
            {
                golbal_flow.acculat = 0;
            }
        }
        else
        {
            golbal_flow.instant  = 0;
        }
                
        //printf("瞬间流量：%.2f（L/min） 累计流量：%.2f（L）\n",golbal_flow.instant,golbal_flow.acculat);
				
				
                
        golbal_flow.receive_flag = 0;                  //接收完成标志位清零
    
    golbal_flow.pluse_1s = 0;
        
        //LED0=!LED0;   //LED1灯 取反提示系统正常运行
    }
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //①时钟 TIM3 使能
    
    //定时器 TIM3 初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化 TIM3
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //允许更新中断
    
    //中断优先级 NVIC 设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级 3 级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
    NVIC_Init(&NVIC_InitStructure); //④初始化 NVIC 寄存器
    TIM_Cmd(TIM3, ENABLE); //⑤使能 TIM3
}


//定时器 3 中断服务程序
void TIM3_IRQHandler(void) //TIM3 中断
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查 TIM3 更新中断发生与否
    {
       
				
				// 每中断一次查询一次流量
				Flow_Read();
        golbal_flow.receive_flag = 1;
				OLED_ShowNum(40,16,golbal_flow.instant,3,16,1);
				
				OLED_Refresh();
			
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //清除 TIM3 更新中断标志
			
    }
}
