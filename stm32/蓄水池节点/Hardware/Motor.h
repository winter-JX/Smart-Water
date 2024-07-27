#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_SetSpeed(int8_t Speed);
void Motor21_SetSpeed(int8_t Speed);
void Motor22_SetSpeed(int8_t Speed);

void Motor1_ON();
void Motor1_OFF();
void Motor11_ON();
void Motor11_OFF();
void Motor12_ON();
void Motor12_OFF();
void Motor2_ON();
void Motor2_OFF();

#endif
