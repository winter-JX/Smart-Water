#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "AD.h"
#include "Serial.h"
#include "string.h"
#include "pwm.h"
#include "FLOW.h"
#include "Motor.h"
#include "EXIT.h"


// ��ʱ����FLOW.C


uint16_t AD0, AD1, AD2, AD3;
uint8_t i;  //�������
uint8_t mode;
uint8_t mode_ex;
uint8_t num_jing;
uint8_t i_jing;
uint8_t tmp_jing;
uint8_t Serial_TxFlag;
uint32_t tmp_flow;

int main(void)
{
	delay_init();
	LED_Init();
	OLED_Init();
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
  OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	AD_Init();
	Serial_Init();
	PWM_Init();
	
	//---------------------------------------------------------
	Motor_Init();		//ֱ�������ʼ��
	int Speed = 0;		//�����ٶȱ���
	Motor2_ON();
	uint8_t M1_Flag = 0;
	uint8_t M11_Flag = 0;
	uint8_t M12_Flag = 0;
	//---------------------------------------------------------
	
	Exit2_Config();            //�ⲿ�жϼ���������
	ExitKey_Config();
  TIM3_Int_Init(9999,7199);    //��ʱ1S
	
	
	uint8_t dataBytes[][3] = {
		{0x00, 0x01, 0x18}, // kong
    {0x00, 0x01, 0x18}, //1
    {0x00, 0x02, 0x19},  
    {0x00, 0x05, 0x22}, //3
		{0x00, 0x06, 0x23},  
    {0x00, 0x08, 0x25}, //5
		{0x00, 0x03, 0x20},  
    {0x00, 0x07, 0x24}, //7
		{0x00, 0x04, 0x21}
	};
	uint16_t length = sizeof(dataBytes) / sizeof(dataBytes[0]); // �������鳤��
	
	
	
	mode = 0; //Ĭ��ģʽ0
	mode_ex = 0; //��ִ��ģʽ����
	num_jing = 1; //�����ģʽ�µ���ģʽ�����������
	i_jing = 1; //�����
	Serial_TxFlag = 0;
	Serial_RxFlag2 = 0;
	
	//---------------------------------------------------------
	//�̶���ʾ
	OLED_ShowNum(95,15,mode,1,16,1);
	OLED_ShowChinese(0,0,0,16,1); //16*16 ˮ
	OLED_ShowChinese(15,0,1,16,1); //16*16 λ
	OLED_ShowString(31,0,":",16,1);//16*16 :
	
	OLED_ShowChinese(0,15,2,16,1); //16*16 ��
	OLED_ShowChinese(15,15,3,16,1); //16*16 ��
	OLED_ShowString(31,15,":",16,1);//16*16 :
	OLED_Refresh();
	
	
	while (1)
	{
		
		
		//AD0 = AD_GetValue(ADC_Channel_0);
		AD1 = AD_GetValue(ADC_Channel_1);

		//OLED_ShowNum(40,15,AD1,4,16,1);
		//OLED_ShowNum(40,15,g_flow,4,16,1);
		
		//delay_ms(100);
		//OLED_Refresh();
		
		
		if (1)
		{
			//Serial_SendArray(dataBytes[0], length); // ʹ��Serial_SendArray�����������ֽ�
			//Serial_SendString("wait\r\n");
			
			//OLED_ShowNum(100,15,1,1,16,1);
			//Serial_SendString(USART3, "ack\r\n");
			//delay_ms(1000);
		}
		
		//***************************************************************************************
		// ������λ��ָ��
		
		if (Serial_RxFlag3 == 1)
		{
			if(Serial_RxPacket3[0] == 'i')
			{
				// ��ָ��
				Serial_SendString(USART3, "!ack\r\n");
				mode = Serial_RxPacket3[1] - '0';
				num_jing = Serial_RxPacket3[2] - '0';
				i_jing =  Serial_RxPacket3[3] - '0';
				mode_ex = Serial_RxPacket3[4] - '0';
				OLED_ShowNum(95,15,mode,1,16,1);
				OLED_ShowNum(110,15,num_jing,1,16,1);
				OLED_Refresh();
				LED2_Turn();
			}
			else if(Serial_RxPacket3[0] == '0')
			{
				// ������ˮ��֮��ˮ��
				Serial_SendString(USART3, "!ack\r\n");
				LED2_Turn();
			}
			else if(Serial_RxPacket3[0] == '1')
			{
				// ��ѯ���ˮ����
				Serial_SendString(USART3, "ack\r\n");
				tmp_flow = golbal_flow.instant;
				Serial_SendNumber(USART3, golbal_flow.instant, 8);
				LED2_Turn();
			}
			else if(Serial_RxPacket3[0] == '2')
			{
				// �������ˮ����
				tmp_jing = Serial_RxPacket3[1] - '0';
				tmp_jing = tmp_jing*10 + Serial_RxPacket3[2] - '0';
				Speed = tmp_jing;
				Speed %= 100;
				Motor_SetSpeed(Speed);				//����ֱ��������ٶ�Ϊ�ٶȱ���
				OLED_ShowNum(31,0,Speed,3,16,1);
				OLED_Refresh();
				
				Serial_SendString(USART3, "!ack\r\n");
				LED2_Turn();
			}
			
			else if(Serial_RxPacket3[0] == 'n')
			{
				// �Զ�Ӧˮ���·�������
				tmp_jing = Serial_RxPacket3[1] - '0';
				Serial_SendArray(USART1, dataBytes[tmp_jing], length); 
				Serial_SendString(USART1, "!on\r\n");
				
				Serial_SendString(USART3, "!ack\r\n");
				LED2_Turn();
			}
			else if(Serial_RxPacket3[0] == 'f')
			{
				// �Զ�Ӧˮ���·�������
				tmp_jing = Serial_RxPacket3[1] - '0';
				Serial_SendArray(USART1, dataBytes[tmp_jing], length); 
				Serial_SendString(USART1, "!on\r\n");
				
				Serial_SendString(USART3, "!ack\r\n");
				LED2_Turn();
			}
			else if(Serial_RxPacket3[0] == 'c')
			{
				// �Զ�Ӧˮ���·���ѯ����
				tmp_jing = Serial_RxPacket3[1] - '0';
				Serial_SendArray(USART1, dataBytes[tmp_jing], length); 
				Serial_SendString(USART1, "!check\r\n");
				Serial_TxFlag = 2;  // ��־�ѷ���
				
				Serial_SendString(USART3, "!ack\r\n");
				LED2_Turn();
			}
			
			Serial_RxFlag3 = 0;
		}
		//***************************************************************************************
			
		//***************************************************************************************
		// ����ˮ���ڵ㷵�ص���Ϣ
		if (Serial_TxFlag == 1)
		{
			if (Serial_RxFlag == 1)
			{
				
				// ��������������ת������λ��
				Serial_SendString(USART3, Serial_RxPacket);
				Serial_SendString(USART3, "\r\n");
				
				Serial_RxFlag = 0;
			}
		}
		//***************************************************************************************
		
		
		//***************************************************************************************
		// ��ˮ���ڵ㷢��ָ�� & �´���ˮ��ָ��
		if(mode_ex == 1){
			if(mode == 0){        // ��ʼ������
				//------------------------------------------------
				// 0:����ˮλ������
				// 1;
				// 2:
				// 3:
					if(num_jing == 0)  
					{
						Serial_SendString(USART2, "CAL\r\n");
					}
					
					
			}
			
			else if(mode == 1){        // ������ˮ��ˮ��
				//------------------------------------------------
				// 0:���忪��
				// 1;����1
				// 2:����2
				// 3:�л���������һ��ʱ����ʹ�ã�
					if(num_jing == 0)  
					{
						if(M1_Flag)  
						{
							Serial_SendString(USART3, "!M1_OFF\r\n");
							Motor1_OFF();
						}
						else {
							Motor1_ON();
							Serial_SendString(USART3, "!M1_ON\r\n");
						}
						
						if(M1_Flag)  M1_Flag = 0;
						else M1_Flag = 1;
						
					}
					
					if(num_jing == 1)
					{
						if(M11_Flag)  
						{
							Serial_SendString(USART3, "!M11_OFF\r\n");
							Motor11_OFF();
						}
						else {
							Serial_SendString(USART3, "!M11_ON\r\n");
							Motor11_ON();
						}
						
						if(M11_Flag)  M11_Flag = 0;
						else M11_Flag = 1;
						
					}
					
					if(num_jing == 2)
					{
						if(M12_Flag)  {
							Serial_SendString(USART3, "!M12_OFF\r\n");
							Motor12_OFF();
						}
						else{
							Serial_SendString(USART3, "!M12_ON\r\n");
							Motor12_ON();
						}
						
						if(M12_Flag)  M12_Flag = 0;
						else M12_Flag = 1;
						
					}
					
					if(num_jing == 3)
					{
						if(M11_Flag == 1 && M12_Flag == 0)  
						{
								Motor11_OFF();
								M11_Flag = 0;
								Motor12_ON();
								M12_Flag = 1;
						}
						if(M11_Flag == 0 && M12_Flag == 1)  
						{
								Motor12_OFF();
								M12_Flag = 0;
								Motor11_ON();
								M11_Flag = 1;
						}	
					}
			}
			
			
			else if(mode == 2){  // �����������
				//------------------------------------------------
				// 0:����ֵ  pass
				// 1;+5
				// 2:-5
				// 3:��һ�����
				// 4:���������
				if(num_jing == 1)  
					{
						Speed += 5;
						Speed %= 100;
						OLED_ShowNum(100,0,Speed,2,16,1);
						OLED_Refresh();
					}
					
				else if(num_jing == 2)  
					{
						Speed -= 5;
						Speed %= 100;
						OLED_ShowNum(100,0,Speed,2,16,1);
						OLED_Refresh();
					}
				else if(num_jing == 3)  
				{
					Motor21_SetSpeed(Speed);
				}
				else if(num_jing == 4)  
				{
					Motor22_SetSpeed(Speed);
				}
			}
			
			else if(mode == 3){  // ��ˮ���·�����
				//------------------------------------------------
				// 0:check
				// 1;��
				// 2:�ر�
				if(num_jing == 0)  
					{
						Serial_SendArray(USART1, dataBytes[i_jing], length); // ʹ��Serial_SendArray�����������ֽ�
						Serial_SendString(USART1, "!check\r\n");
						Serial_TxFlag = 1;  // ��־�ѷ���
					}
					
				if(num_jing == 1)  
					{
						Serial_SendArray(USART1, dataBytes[i_jing], length); 
						Serial_SendString(USART1, "!on\r\n");
						//Serial_TxFlag = 1;  // ��־�ѷ���
					}
				if(num_jing == 2)  
					{
						Serial_SendArray(USART1, dataBytes[i_jing], length); 
						Serial_SendString(USART1, "!off\r\n");
						//Serial_TxFlag = 1;  // ��־�ѷ���
					}
					
			}
			
			
			mode_ex = 0;
		}
		//***************************************************************************************
		
		
		
		

		
		
	}
}
