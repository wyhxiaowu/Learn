#ifndef __TIM_H
#define	__TIM_H


#include "stm32f10x.h"

#define		General_TIM           	    TIM3
#define		TIM_IRQn                    TIM3_IRQn
#define		TIM_CLK                     RCC_APB1Periph_TIM3
#define	    General_TIM_Period		    (5000-1)
#define	    General_TIM_Prescaler	    (72-1)


void GeneralTim_Init(void);

#endif
