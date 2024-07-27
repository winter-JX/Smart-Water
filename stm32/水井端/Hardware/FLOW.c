#include "FLOW.h"
#include "OLED.h"

GOLBAL_FLOW golbal_flow;

#define Flow_Model_4       1            //4�ֹܶ���Ϊ1��6�ֹܶ���Ϊ0
#define MODE_4_K               62.0f
#define MODE_4_PLUSE_CNT_1L         3720.0f
#define MODE_6_K               5.5f
#define MODE_6_PLUSE_CNT_1L    330.0f
#define    FLOW_FULL                         1000000


//��PA7���ⲿ�ж�����ȡ�͵�ƽ������
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
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //�½��ش����ж�
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
		//����GPIO AFIO��ʱ�ӿ�������
		//EXIT��NVIC���ں����裩������Ҫ��������
		
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//��������
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//Ĭ�ϼ���
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		//����GPIO�˿�
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
		//����AFIO�����ж��������裨ע��AFIO�����ļ���GPIO�ļ��
		
		EXTI_InitTypeDef EXTI_InitStruct;
		EXTI_InitStruct.EXTI_Line=EXTI_Line14;//ѡ���ж���
		EXTI_InitStruct.EXTI_LineCmd=ENABLE;//ѡ��EXTI��״̬��ʹ��/ʧ��
		EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//ѡ��EXTI��ģʽ���ж���Ӧ/�¼���Ӧ
		EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//ָ��EXTI�ߵĴ����źż����Ե��������/�½���/˫���أ�ע���⺯���У������Ӧ����EXTITrigger_TypeDef��
		EXTI_Init(&EXTI_InitStruct);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;//ָ��Ҫ���û���õ�IRQͨ��
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//ʹ�ܻ�ʧ��
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//ָ��NVIC_IRQChannel��ָ����IRQͨ������ռ���ȼ�
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;//ָ��NVIC_IRQChannel��ָ����IRQͨ������Ӧ���ȼ�
		NVIC_Init(&NVIC_InitStruct);
}
  
//void EXTI2_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
//  {
//    golbal_flow.pluse_1s++;           //�½��زɼ�������
//  }
//  EXTI_ClearITPendingBit(EXTI_Line2); //���Line2���жϱ�־λ
//}


void EXTI15_10_IRQHandler(void)//�жϺ����ĸ�ʽ
{
	if (EXTI_GetITStatus(EXTI_Line14)==SET)//�ж��Ƿ�ΪEXTI14����������ֵΪ SET or RESET
	{
		golbal_flow.pluse_1s++;
		EXTI_ClearITPendingBit(EXTI_Line14);//����жϺ����ı�־λ���������һֱ�����жϺ�����
	}
	
}

void Flow_Read(void)
{
		int g_flow = 0;  // ˲ʱ��������
	
    if(golbal_flow.receive_flag)
    {
        if(golbal_flow.pluse_1s > 0)
        {
      #ifdef Flow_Model_4
            /*���㹫ʽ��
                �ۼ����� = ��˲ʱ����������
                                 = (����Ƶ�� / 3720������)    //1L��Ҫ3720�����壬����Ƶ��HZ
            */
            golbal_flow.acculat += golbal_flow.pluse_1s / MODE_4_PLUSE_CNT_1L * 1000;   //��λmL
                
            /*���㹫ʽ��
                        ˲ʱ���� = (����Ƶ�� / 3720������) * 60s 
                                         = ����Ƶ�� / 62.0(����ϵK)
            */
            golbal_flow.instant = golbal_flow.pluse_1s / MODE_4_K * 1000;  //��λ��mL/min��
      
      #else
          /*���㹫ʽ��
                �ۼ����� = ��˲ʱ����������
                                 = (����Ƶ�� / 330������)    //1L��Ҫ330�����壬����Ƶ��HZ
            */
            golbal_flow.acculat += golbal_flow.pluse_1s / MODE_6_PLUSE_CNT_1L;   //��λL
                
            /*���㹫ʽ��
                        ˲ʱ���� = ((����Ƶ�� + 3) / 330������) * 60s 
                                         = (����Ƶ�� + 3) / 5.5(����ϵK)
            */
            golbal_flow.instant = (golbal_flow.pluse_1s + 3) / MODE_6_K;  //��λ��L/min��
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
                
        //printf("˲��������%.2f��L/min�� �ۼ�������%.2f��L��\n",golbal_flow.instant,golbal_flow.acculat);
				
				
                
        golbal_flow.receive_flag = 0;                  //������ɱ�־λ����
    
    golbal_flow.pluse_1s = 0;
        
        //LED0=!LED0;   //LED1�� ȡ����ʾϵͳ��������
    }
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��ʱ�� TIM3 ʹ��
    
    //��ʱ�� TIM3 ��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ�� TIM3
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //��������ж�
    
    //�ж����ȼ� NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 �ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 0 ��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 3 ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); //�ܳ�ʼ�� NVIC �Ĵ���
    TIM_Cmd(TIM3, ENABLE); //��ʹ�� TIM3
}


//��ʱ�� 3 �жϷ������
void TIM3_IRQHandler(void) //TIM3 �ж�
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //��� TIM3 �����жϷ������
    {
       
				
				// ÿ�ж�һ�β�ѯһ������
				Flow_Read();
        golbal_flow.receive_flag = 1;
				OLED_ShowNum(40,16,golbal_flow.instant,3,16,1);
				
				OLED_Refresh();
			
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //��� TIM3 �����жϱ�־
			
    }
}
