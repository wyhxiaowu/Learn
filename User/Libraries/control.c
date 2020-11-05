#include "headfile.h"

HALL_SpeedDef   HALL_HighSpeedStruct = {0,0,0,0};
HALL_SpeedDef   HALL_LowSpeedStruct = {0,0,0,0};

/*--------------------高速测速法（M法）-------------------*/	 
void HighSpeed_Measure(void)
{
    HALL_HighSpeedStruct.Hall1_Speed = Hall1_Count/(50*(HALL_TIM_Period+1)*(HALL_TIM_Prescaler+1)/72000000.0);
    HALL_HighSpeedStruct.Hall2_Speed = Hall2_Count/(50*(HALL_TIM_Period+1)*(HALL_TIM_Prescaler+1)/72000000.0);
    HALL_HighSpeedStruct.Hall3_Speed = Hall3_Count/(50*(HALL_TIM_Period+1)*(HALL_TIM_Prescaler+1)/72000000.0);
    HALL_HighSpeedStruct.Hall_Speed = (HALL_HighSpeedStruct.Hall1_Speed + HALL_HighSpeedStruct.Hall2_Speed + HALL_HighSpeedStruct.Hall3_Speed) / 3.0;
}

/*--------------------低速测速法（T法）-------------------*/	
void LowSpeed_Measure(void)
{
    if(TIM_ICUserValueStruct_I.Capture_FinishFlag == 1)
    {
        HALL_LowSpeedStruct.Hall1_Speed = 1/(50.0*Hall1_time);
        TIM_ICUserValueStruct_I.Capture_FinishFlag = 0;
    }
    if(TIM_ICUserValueStruct_II.Capture_FinishFlag == 1)
    {
        HALL_LowSpeedStruct.Hall2_Speed = 1/(50.0*Hall2_time);
        TIM_ICUserValueStruct_II.Capture_FinishFlag = 0;
    }
    if(TIM_ICUserValueStruct_III.Capture_FinishFlag == 1)
    {
        HALL_LowSpeedStruct.Hall3_Speed = 1/(50.0*Hall3_time);
        TIM_ICUserValueStruct_III.Capture_FinishFlag = 0;
    }
    
    HALL_LowSpeedStruct.Hall_Speed = (HALL_LowSpeedStruct.Hall1_Speed + HALL_LowSpeedStruct.Hall2_Speed + HALL_LowSpeedStruct.Hall3_Speed) / 3.0;
}

/*--------------------串口打印程序-------------------*/	
void OutPutSomething(void)
{
    printf("\rHall1 Speed contrast... HighSpeed:%f n/s    LowSpeed:%f n/s \r",HALL_HighSpeedStruct.Hall1_Speed,HALL_LowSpeedStruct.Hall1_Speed);
    printf("Hall2 Speed contrast... HighSpeed:%f n/s    LowSpeed:%f n/s \r",HALL_HighSpeedStruct.Hall2_Speed,HALL_LowSpeedStruct.Hall2_Speed);
    printf("Hall3 Speed contrast... HighSpeed:%f n/s    LowSpeed:%f n/s \r",HALL_HighSpeedStruct.Hall3_Speed,HALL_LowSpeedStruct.Hall3_Speed);
    printf("High_Low Speed contrast... HighSpeed:%f n/s    LowSpeed:%f n/s \r",HALL_HighSpeedStruct.Hall_Speed,HALL_LowSpeedStruct.Hall_Speed);  
    if(Direction == 1)  printf("Direction is -------> \r");
    else if(Direction == 2)  printf("Direction is <------- \r");
    else printf("Direction is Loading \r\n") ; 
}

