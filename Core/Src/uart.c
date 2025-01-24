#include "uart.h"

/**
 * @brief	串口1初始化函数--tim1
 *
 * @param   
 *
 * @return  void
**/
void Uart1_Init( void )     //115200bps@11.0592MHz
{	
	P_SW1 &= 0X3F;          //选择P3.0 P3.1为UART引脚
	
	SCON  = 0x50;           //8位数据,可变波特率

	AUXR &= 0xFE;           //串口1选择定时器1为波特率发生器 S1ST2 = 0
	AUXR |= 0x40;           //定时器时钟1T模式

	TL1   = 0xE8;			//设置定时初始值
	TH1   = 0xFF;			//设置定时初始值

	TCON |= 0X40;			//定时器1开始计时

    IE   |= 0X10;			//串口1 中断允许位
}

/**
 * @brief	串口2初始化函数--tim2
 *
 * @param   
 *
 * @return  void
**/
void Uart2_Init( void )       //115200bps@11.0592MHz
{
    P_SW2 = 0X00;          //选择P3.0 P3.1为UART引脚

	S2CON = 0x50;           //8位数据,可变波特率

	AUXR |= 0x04;           //定时器时钟1T模式

	T2L   = 0xE8;           //设置定时初始值
	T2H   = 0xFF;           //设置定时初始值

	AUXR |= 0x10;           //定时器2开始计时

	IE2  |= 0X01;			//串口2 中断允许位
}

