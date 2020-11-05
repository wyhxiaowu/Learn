#include "LED.h"

void LED_Init_Config(void)
{
	GPIO_InitTypeDef    GPIO_InitTypeStruct;
	
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK,ENABLE);
	
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeStruct.GPIO_Pin = LED_GPIO_PIN;
	GPIO_Init(LED_GPIO_PORT,&GPIO_InitTypeStruct);
	
	GPIO_SetBits(LED_GPIO_PORT,LED_GPIO_PIN);
}
