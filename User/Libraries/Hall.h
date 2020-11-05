#ifndef __HALL_H
#define	__HALL_H

#include "stm32f10x.h"

extern uint8_t Hall_Code[3],Direction;
extern uint32_t Hall1_Count,Hall2_Count,Hall3_Count;
extern float Hall1_time,Hall2_time,Hall3_time; 

// 定时器输入捕获用户自定义变量结构体声明
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // 捕获结束标志位
	uint8_t   Capture_StartFlag;    // 捕获开始标志位
	uint32_t  Capture_CcrValue;     // 捕获寄存器的值
	uint16_t  Capture_Period;       // 自动重装载寄存器更新标志 
}TIM_ICUserValueTypeDef;

extern TIM_ICUserValueTypeDef TIM_ICUserValueStruct_I;
extern TIM_ICUserValueTypeDef TIM_ICUserValueStruct_II;
extern TIM_ICUserValueTypeDef TIM_ICUserValueStruct_III;

#define		HALL_TIM           	 	   TIM2
#define		HALL_TIM_IRQn              TIM2_IRQn
#define		HALL_TIM_CLK               RCC_APB1Periph_TIM2

#define		HALL_TIM_GPIO_CLK          RCC_APB2Periph_GPIOA
#define		HALL_TIM_PORT          	   GPIOA
#define		HALL_TIM_CH2_PIN           GPIO_Pin_1
#define		HALL_TIM_CH3_PIN           GPIO_Pin_2
#define		HALL_TIM_CH4_PIN           GPIO_Pin_3

#define		HALL_CH2_CHANNEL           TIM_Channel_2
#define		HALL_CH3_CHANNEL           TIM_Channel_3
#define		HALL_CH4_CHANNEL           TIM_Channel_4

#define	    HALL_TIM_Period			   (5000-1)
#define	    HALL_TIM_Prescaler		   (72-1)

void Hall_Init(void);
void Capture_Period_Config(void);
void Hall1_PulseWidth_Measure(void);
void Hall2_PulseWidth_Measure(void);
void Hall3_PulseWidth_Measure(void);
void RotationDirection(void);
void Data_Acquisition(void);
void CleanFlag(void);

#endif
