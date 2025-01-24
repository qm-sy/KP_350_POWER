#include "tim.h"

void Timer0_Init( void )		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0   = 0xCD;           //设置定时初始值
	TH0   = 0xD4;           //设置定时初始值
	TF0   = 0;				//清除TF0标志
	TR0   = 1;				//定时器0开始计时

    ET0   = 1;              //打开IE-ET0，TIM0中断
}