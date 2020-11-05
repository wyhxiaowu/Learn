/*-------------------------------
输入捕获定时器设置
-------------------------------*/
#include "Hall.h"

uint8_t Hall_Code[3],Last_Hall_Code[3],Direction;
uint32_t Hall1_Count,Hall2_Count,Hall3_Count;
float Hall1_time,Hall2_time,Hall3_time; 

TIM_ICUserValueTypeDef TIM_ICUserValueStruct_I =  {0,0,0,0};
TIM_ICUserValueTypeDef TIM_ICUserValueStruct_II = {0,0,0,0};
TIM_ICUserValueTypeDef TIM_ICUserValueStruct_III= {0,0,0,0};


static void TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef    NVIC_InitStruct;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStruct.NVIC_IRQChannel = HALL_TIM_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStruct);
}

static void TIM_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  // 输入捕获通道 GPIO 初始化
  RCC_APB2PeriphClockCmd(HALL_TIM_GPIO_CLK, ENABLE);
  GPIO_InitStruct.GPIO_Pin =  HALL_TIM_CH2_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(HALL_TIM_PORT, &GPIO_InitStruct);	

	GPIO_InitStruct.GPIO_Pin =  HALL_TIM_CH3_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(HALL_TIM_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =  HALL_TIM_CH4_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(HALL_TIM_PORT, &GPIO_InitStruct);	
}

static void TIM_Mode_Config(void)
{
  RCC_APB1PeriphClockCmd(HALL_TIM_CLK,ENABLE);
/*--------------------时基结构体初始化-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStruct.TIM_Period=HALL_TIM_Period;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStruct.TIM_Prescaler= HALL_TIM_Prescaler;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(HALL_TIM, &TIM_TimeBaseStruct);

	/*--------------------输入捕获结构体初始化-------------------*/	 
  TIM_ICInitTypeDef  TIM_ICInitStruct;
  TIM_ICInitStruct.TIM_Channel = HALL_CH2_CHANNEL;
  TIM_ICInitStruct.TIM_ICFilter = 0;
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInit(HALL_TIM,&TIM_ICInitStruct);

  TIM_ICInitStruct.TIM_Channel = HALL_CH3_CHANNEL;
  TIM_ICInitStruct.TIM_ICFilter = 0;
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInit(HALL_TIM,&TIM_ICInitStruct);

  TIM_ICInitStruct.TIM_Channel = HALL_CH4_CHANNEL;
  TIM_ICInitStruct.TIM_ICFilter = 0;
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInit(HALL_TIM,&TIM_ICInitStruct);  

	// 清除更新和捕获中断标志位
  TIM_ClearFlag(HALL_TIM, TIM_FLAG_Update|TIM_FLAG_CC2|TIM_FLAG_CC3|TIM_FLAG_CC4);	
  // 开启更新和捕获中断  
	TIM_ITConfig (HALL_TIM, TIM_IT_Update|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);	
	// 使能计数器
	TIM_Cmd(HALL_TIM, ENABLE);

}
	
/*--------------------HAll初始化-------------------*/	 
void Hall_Init(void)
{
  TIM_NVIC_Config();
  TIM_Mode_Config();
  TIM_GPIO_Config();
}

/*--------------------Hall时钟因子计数初始化---------------*/
void Capture_Period_Config(void)
{
	if ( TIM_GetITStatus ( HALL_TIM, TIM_IT_Update) != RESET )               
	{	
		TIM_ICUserValueStruct_I.Capture_Period ++;	
    TIM_ICUserValueStruct_II.Capture_Period++;
    TIM_ICUserValueStruct_III.Capture_Period++;	
		TIM_ClearITPendingBit ( HALL_TIM, TIM_FLAG_Update ); 		
	}
}

/*--------------------Hall脉宽及计数检测-------------------
TIM_ICPolarity_Falling == TIM_OCPolarity_Low
TIM_ICPolarity_Rising == TIM_OCPolarity_High
一个周期对应一个齿所以两个下降沿进行一次计数和计时
----------------------------------------------------*/	
void Hall1_PulseWidth_Measure(void)
{
  if(TIM_GetITStatus(HALL_TIM,TIM_IT_CC2) != RESET)
  {
    //第一次捕获
    if(TIM_ICUserValueStruct_I.Capture_StartFlag == 0)
    {
			// 计数器清0
			TIM_SetCounter ( HALL_TIM, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStruct_I.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStruct_I.Capture_CcrValue = 0;
			// 当第一次捕获到下降沿之后，捕获边沿仍为下降沿
			TIM_OC2PolarityConfig(HALL_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStruct_I.Capture_StartFlag = 1;	
    }
    //上升沿捕获
    else
    {
			// 获取捕获比较寄存器的值，这个值就是捕获到的两次触发hall的时间的值
			TIM_ICUserValueStruct_I.Capture_CcrValue = TIM_GetCapture2 (HALL_TIM); 
      // 当第二次捕获之后，就把捕获边沿配置为下降沿，好开启新的一轮捕获
			TIM_OC2PolarityConfig(HALL_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标志清0		
			TIM_ICUserValueStruct_I.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStruct_I.Capture_FinishFlag = 1;	
      //单齿计数
      Hall1_Count++;
    } 	
    TIM_ClearITPendingBit (HALL_TIM,TIM_IT_CC2);	
		Hall1_time = TIM_ICUserValueStruct_I.Capture_Period * 72 + (TIM_ICUserValueStruct_I.Capture_CcrValue+1); 
  }
}

void Hall2_PulseWidth_Measure(void)
{
  if(TIM_GetITStatus(HALL_TIM,TIM_IT_CC3) != RESET)
  {
    //第一次捕获
    if(TIM_ICUserValueStruct_II.Capture_StartFlag == 0)
    {
			// 计数器清0
			TIM_SetCounter ( HALL_TIM, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStruct_II.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStruct_II.Capture_CcrValue = 0;
			// 当第一次捕获到下降沿之后，捕获边沿仍为下降沿
			TIM_OC3PolarityConfig(HALL_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStruct_II.Capture_StartFlag = 1;	
    }
    //上升沿捕获
    else
    {
			// 获取捕获比较寄存器的值，这个值就是捕获到的两次触发hall的时间的值
			TIM_ICUserValueStruct_II.Capture_CcrValue = TIM_GetCapture3 (HALL_TIM); 
      // 当第二次捕获之后，就把捕获边沿配置为下降沿，好开启新的一轮捕获
			TIM_OC3PolarityConfig(HALL_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标志清0		
			TIM_ICUserValueStruct_II.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStruct_II.Capture_FinishFlag = 1;	
      //单齿计数
      Hall2_Count++;
    } 	
    TIM_ClearITPendingBit (HALL_TIM,TIM_IT_CC3);	
		Hall2_time = TIM_ICUserValueStruct_II.Capture_Period * 72 + (TIM_ICUserValueStruct_II.Capture_CcrValue+1); 
  }
}

void Hall3_PulseWidth_Measure(void)
{
  if(TIM_GetITStatus(HALL_TIM,TIM_IT_CC4) != RESET)
  {
    //第一次捕获
    if(TIM_ICUserValueStruct_III.Capture_StartFlag == 0)
    {
			// 计数器清0
			TIM_SetCounter ( HALL_TIM, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStruct_III.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStruct_III.Capture_CcrValue = 0;
			// 当第一次捕获到下降沿之后，捕获边沿仍为下降沿
			TIM_OC4PolarityConfig(HALL_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStruct_III.Capture_StartFlag = 1;	
    }
    //上升沿捕获
    else
    {
			// 获取捕获比较寄存器的值，这个值就是捕获到的两次触发hall的时间的值
			TIM_ICUserValueStruct_III.Capture_CcrValue = TIM_GetCapture4 (HALL_TIM); 
      // 当第二次捕获之后，就把捕获边沿配置为下降沿，好开启新的一轮捕获
			TIM_OC4PolarityConfig(HALL_TIM, TIM_ICPolarity_Falling);
      // 开始捕获标志清0		
			TIM_ICUserValueStruct_III.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStruct_III.Capture_FinishFlag = 1;	
      //单齿计数
      Hall3_Count++;
    } 	
    TIM_ClearITPendingBit (HALL_TIM,TIM_IT_CC4);	
		Hall3_time = TIM_ICUserValueStruct_III.Capture_Period * 72 + (TIM_ICUserValueStruct_III.Capture_CcrValue+1); 
  }
}

/*--------------------三相脉宽计数检测-------------------*/
void Data_Acquisition(void)
{
  Capture_Period_Config();
  Hall1_PulseWidth_Measure();
  Hall2_PulseWidth_Measure();
  Hall3_PulseWidth_Measure();
  //单位转换 us------->s
  Hall1_time = Hall1_time / 1000000.0;
  Hall2_time = Hall2_time / 1000000.0;
  Hall3_time = Hall3_time / 1000000.0;
}

/*--------------------方向判断程序-------------------*/	
void RotationDirection(void)
{
  if((TIM_GetITStatus(HALL_TIM,TIM_IT_CC2) != RESET) || (TIM_GetITStatus(HALL_TIM,TIM_IT_CC3) != RESET) || (TIM_GetITStatus(HALL_TIM,TIM_IT_CC4) != RESET))
  {
    Hall_Code[0] = GPIO_ReadInputDataBit(GPIOA,HALL_TIM_CH2_PIN);
    Hall_Code[1] = GPIO_ReadInputDataBit(GPIOA,HALL_TIM_CH3_PIN);
    Hall_Code[2] = GPIO_ReadInputDataBit(GPIOA,HALL_TIM_CH4_PIN);
    if (Hall_Code[0] == 0 && Hall_Code[1] == 0 && Hall_Code[2] == 0)
    {
      if (Last_Hall_Code[0] == 0 && Last_Hall_Code[1] == 0 && Last_Hall_Code[2] == 1)   Direction = 1;
      else if (Last_Hall_Code[0] == 1 && Last_Hall_Code[1] == 0 && Last_Hall_Code[2] == 0)  Direction = 2;   
    }
    else if(Hall_Code[0] == 0 && Hall_Code[1] == 0 && Hall_Code[2] == 1)
    {
      if (Last_Hall_Code[0] == 0 && Last_Hall_Code[1] == 1 && Last_Hall_Code[2] == 1)   Direction = 1;
      else if (Last_Hall_Code[0] == 0 && Last_Hall_Code[1] == 0 && Last_Hall_Code[2] == 0)  Direction = 2;
    }
    else if(Hall_Code[0] == 0 && Hall_Code[1] == 1 && Hall_Code[2] == 1)
    {
      if (Last_Hall_Code[0] == 1 && Last_Hall_Code[1] == 1 && Last_Hall_Code[2] == 1)   Direction = 1;
      else if (Last_Hall_Code[0] == 0 && Last_Hall_Code[1] == 0 && Last_Hall_Code[2] == 1)  Direction = 2;
    }    
    else if(Hall_Code[0] == 1 && Hall_Code[1] == 1 && Hall_Code[2] == 1)
    {
      if (Last_Hall_Code[0] == 1 && Last_Hall_Code[1] == 1 && Last_Hall_Code[2] == 0)   Direction = 1;
      else if (Last_Hall_Code[0] == 0 && Last_Hall_Code[1] == 1 && Last_Hall_Code[2] == 1)  Direction = 2;
    }  
    else if(Hall_Code[0] == 1 && Hall_Code[1] == 1 && Hall_Code[2] == 0)
    {
      if (Last_Hall_Code[0] == 1 && Last_Hall_Code[1] == 0 && Last_Hall_Code[2] == 0)   Direction = 1;
      else if (Last_Hall_Code[0] == 1 && Last_Hall_Code[1] == 1 && Last_Hall_Code[2] == 1)  Direction = 2;
    }  
    else if(Hall_Code[0] == 1 && Hall_Code[1] == 0 && Hall_Code[2] == 0)
    {
      if (Last_Hall_Code[0] == 0 && Last_Hall_Code[1] == 0 && Last_Hall_Code[2] == 0)   Direction = 1;
      else if (Last_Hall_Code[0] == 1 && Last_Hall_Code[1] == 1 && Last_Hall_Code[2] == 0)  Direction = 2;
    }  
    else;
    
    Last_Hall_Code[0] = Hall_Code[0];Last_Hall_Code[1] = Hall_Code[1];Last_Hall_Code[2] = Hall_Code[2];        
  } 
  
   
}


/*--------------------各标标志位重置-------------------*/
void CleanFlag(void)
{
  Hall1_Count = 0;Hall2_Count = 0;Hall3_Count = 0; //计数清零
}
