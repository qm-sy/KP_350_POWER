#include "uart.h"

/**
 * @brief	串口2初始化函数--tim2
 *
 * @param   
 *
 * @return  void
**/
void Uart2_Init( void )       //115200bps@11.0592MHz
{
    P_SW2 |= 0X00;          //串口2功能脚选择P1.0 P1.1

	S2CON  = 0x50;           //8位数据,可变波特率

	AUXR  |= 0x04;           //定时器时钟1T模式

	T2L    = 0xE8;           //设置定时初始值
	T2H    = 0xFF;           //设置定时初始值

	AUXR  |= 0x10;           //定时器2开始计时

	IE2   |= 0X01;			//串口2 中断允许位
}

/**
 * @brief	串口4初始化函数--tim4
 *
 * @param   
 *
 * @return  void
**/
void Uart4_Init( void )	//115200bps@11.0592MHz
{
	P_SW2 |= 0X04;		//串口4功能脚选择P5.2 P5.3
	
	S4CON  = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器

	T4T3M |= 0x20;		//定时器时钟1T模式

	T4L    = 0xE8;		//设置定时初始值
	T4H    = 0xFF;		//设置定时初始值

	T4T3M |= 0x80;		//定时器4开始计时

	IE2   |= 0x10;		// 串口4中断打开
}
