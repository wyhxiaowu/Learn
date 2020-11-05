#ifndef __CONTROL_H
#define	__CONTROL_H


#include "stm32f10x.h"

#define Max(a,b)	(a>=b?a:b)
#define Min(a,b)	(a<=b?a:b)


typedef struct
{   
	float  Hall1_Speed;   // 霍尔传感器1所得转速
	float  Hall2_Speed;   // 霍尔传感器2所得转速
	float  Hall3_Speed;   // 霍尔传感器3所得转速
	float  Hall_Speed;    // 霍尔传感器平均所得转速 
}HALL_SpeedDef;

extern HALL_SpeedDef HALL_HighSpeedStruct,HALL_LowSpeedStruct;


void HighSpeed_Measure(void);
void LowSpeed_Measure(void);
void OutPutSomething(void);

#endif
