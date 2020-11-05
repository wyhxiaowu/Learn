#ifndef __LED_H
#define	__LED_H

#define LED_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED_GPIO_PIN		GPIO_Pin_1			        /* 连接到SCL时钟线的GPIO */

#define ON   1
#define OFF  0

#define LED(a)    if(a) GPIO_ResetBits(LED_GPIO_PORT,LED_GPIO_PIN);\
								  else  GPIO_SetBits(LED_GPIO_PORT,LED_GPIO_PIN);
									
#define LED_TOGGLE   {LED_GPIO_PORT->ODR ^= LED_GPIO_PIN;} 


void LED_Init_Config(void);

#include "stm32f10x.h"

#endif
