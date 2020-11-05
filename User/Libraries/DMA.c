#include "DMA.h"
#include "control.h"

void HS_DMA_Config(void)
{
    DMA_InitTypeDef     DMA_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA_InitStruct.DMA_BufferSize = SENDBUFF_Size;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&HALL_HighSpeedStruct;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;

    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    // DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;

    DMA_InitStruct.DMA_PeripheralBaseAddr = USART_ADDRESS;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    
    DMA_Init(USART_DMA_Channel,&DMA_InitStruct);
    DMA_Cmd(USART_DMA_Channel,ENABLE);
}

void LS_DMA_Config(void)
{
    DMA_InitTypeDef     DMA_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA_InitStruct.DMA_BufferSize = SENDBUFF_Size;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&HALL_LowSpeedStruct;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;

    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    // DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;

    DMA_InitStruct.DMA_PeripheralBaseAddr = USART_ADDRESS;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    
    DMA_Init(USART_DMA_Channel,&DMA_InitStruct);
    DMA_Cmd(USART_DMA_Channel,ENABLE);
}

