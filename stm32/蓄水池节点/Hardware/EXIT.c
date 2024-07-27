#include "EXIT.h"
#include "LED.h"
#include "OLED.h"

extern uint8_t mode;
extern uint8_t mode_ex;

//��������PA0|PA15|PC5
void ExitKey_Config(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		//����GPIO AFIO��ʱ�ӿ�������
		//EXIT��NVIC���ں����裩������Ҫ��������
		
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;  //��������
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15 ;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//Ĭ�ϼ���
		GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;  //XIA������
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0 ;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//Ĭ�ϼ���
		GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;  //��������
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5 ;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//Ĭ�ϼ���
		GPIO_Init(GPIOC,&GPIO_InitStruct);
		//����GPIO�˿�
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
		//����AFIO�����ж��������裨ע��AFIO�����ļ���GPIO�ļ��
		
		EXTI_InitTypeDef EXTI_InitStruct;
		EXTI_InitStruct.EXTI_Line=EXTI_Line15 | EXTI_Line0 | EXTI_Line5;//ѡ���ж���
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
		
		NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;//ָ��Ҫ���û���õ�IRQͨ��
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//ʹ�ܻ�ʧ��
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//ָ��NVIC_IRQChannel��ָ����IRQͨ������ռ���ȼ�
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//ָ��NVIC_IRQChannel��ָ����IRQͨ������Ӧ���ȼ�
		NVIC_Init(&NVIC_InitStruct);
		
		NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//ָ��Ҫ���û���õ�IRQͨ��
		NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//ʹ�ܻ�ʧ��
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//ָ��NVIC_IRQChannel��ָ����IRQͨ������ռ���ȼ�
		NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;//ָ��NVIC_IRQChannel��ָ����IRQͨ������Ӧ���ȼ�
		NVIC_Init(&NVIC_InitStruct);
}
  
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		LED2_Turn();
		mode_ex = 1;
		
		EXTI_ClearITPendingBit(EXTI_Line0); //���Line0���жϱ�־λ
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
		
		EXTI_ClearITPendingBit(EXTI_Line5); //���Line0���жϱ�־λ
  }
}
