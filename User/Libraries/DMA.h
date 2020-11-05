#ifndef __DMA_H
#define	__DMA_H

#include "stm32f10x.h"

#define USART_DMA_Channel   DMA1_Channel4
#define USART_ADDRESS       (USART1_BASE + 0x04)
#define SENDBUFF_Size       1000


void HS_DMA_Config(void);
void LS_DMA_Config(void);

#endif
