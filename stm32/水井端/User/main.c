#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "Serial.h"
#include "string.h"
#include "pwm.h"
#include "FLOW.h"
#include "Motor.h"

uint16_t AD0, AD1, AD2, AD3;
uint8_t i;  //定义变量
uint8_t init_flag;  //定义变量

int main(void)
{
	delay_init();
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	AD_Init();
	Serial_Init();
	Lora_ON();
	PWM_Init();
	Motor_Init();		//直流电机初始化
	Motor_OFF();
	
	Exit2_Config();            //外部中断检测脉冲个数
  TIM3_Int_Init(9999,7199);    //计时1S
	
	
	uint8_t dataBytes[] = {0x00, 0x00, 0x17}; // 定义包含三个字节的数组
	uint16_t length = sizeof(dataBytes) / sizeof(dataBytes[0]); // 计算数组长度
	
	int8_t Speed;		//定义速度变量
	uint8_t Motor_flag;  //定义变量
	
	
	Motor_OFF();
	Motor_flag  = 0;
	init_flag = 0;
	OLED_ShowString(80,0,"OFF",16,1);//16*16 :
	
	//---------------------------------------------------------
	//固定显示
	OLED_ShowChinese(0,0,1,16,1); //16*16 一
	OLED_ShowChinese(15,0,12,16,1); //16*16 号
	OLED_ShowChinese(31,0,13,16,1); //16*16 井
	OLED_ShowChinese(0,16,10,16,1); //16*16 流
	OLED_ShowChinese(15,16,11,16,1); //16*16 量
	OLED_ShowString(31,16,":",16,1);//16*16 :
	OLED_Refresh();
	
	
	while (1)
	{
		
		if (Serial_RxFlag == 1)
		{
			
			if(strcmp(Serial_RxPacket, "on") == 0)
			{
				if(init_flag){
					Motor_ON();
					OLED_ShowString(80,0,"ON ",16,1);//16*16 
					OLED_Refresh();
					Speed = -100;
					Motor_SetSpeed(-Speed);				//设置直流电机的速度为速度变量
					Motor_flag  = 1;
				}
				else{
					Motor_ON();
					OLED_ShowString(80,0,"ON ",16,1);//16*16 
					OLED_Refresh();
					Speed = -100;
					Motor_SetSpeed(-Speed);				//设置直流电机的速度为速度变量
					Motor_flag  = 1;
					init_flag = 0;
				}
			}
			if(strcmp(Serial_RxPacket, "off") == 0)
			{
				Motor_OFF();
				OLED_ShowString(80,0,"OFF",16,1);//16*16 
				OLED_Refresh();
				Motor_flag  = 0;
			}
			
			if(strcmp(Serial_RxPacket, "check") == 0)
			{
				char send_back[16];
				if(Motor_flag){
					send_back[0] = '1';
				}
				else send_back[0] = '0';
				
				int integerPart_instant = (int)golbal_flow.instant; // 舍去小数部分，转换为整型
				int integerPart_acculat = (int)golbal_flow.acculat; 
				char str[5] = {'0'};
				snprintf(str, sizeof(str), "%05d", integerPart_acculat); 
				int i;
				for(i = 0; i<4; i++) {
						send_back[i + 1] = str[i]; // 同样，+1是为了跳过send_back[0]
				} 
				
				char str2[9] = {'0'};
				snprintf(str2, sizeof(str2), "%08d", integerPart_acculat); 
				
				for(i = 0; i<8; i++) {
						send_back[i + 5] = str2[i]; 
				}
				
				Serial_SendArray(dataBytes, length); // 使用Serial_SendArray发送这三个字节
				Serial_SendString("!");
				Serial_SendString(send_back);
				Serial_SendString("\r\n");
			}
			
			Serial_RxFlag = 0;
		}
		
		
		//***************************************************************************************
		
		
	}
}
