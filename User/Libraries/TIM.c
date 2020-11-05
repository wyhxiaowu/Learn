#include "TIM.h"

static void TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef    NVIC_InitStruct;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStruct.NVIC_IRQChannel = TIM_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStruct);
}

static void TIM_Mode_Config(void)
{
  RCC_APB1PeriphClockCmd(TIM_CLK,ENABLE);
/*--------------------时基结构体初始化-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStruct.TIM_Period=General_TIM_Period;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStruct.TIM_Prescaler= General_TIM_Prescaler;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(General_TIM, &TIM_TimeBaseStruct);
    TIM_ClearFlag(General_TIM, TIM_FLAG_Update);
    TIM_ITConfig(General_TIM, TIM_IT_Update,ENABLE);
    TIM_Cmd(General_TIM, ENABLE);
}

void GeneralTim_Init(void)
{
  TIM_NVIC_Config();
  TIM_Mode_Config();
}
