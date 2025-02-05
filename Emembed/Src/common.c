#include "common.h"

/**
 * @brief	一些定时执行的事件   Timer3：10us
 *
 * @param   
 *
 * @return  void
**/
void Tim3_ISR( void ) interrupt 19 
{
    static uint8_t trun_flag = 1;
    /* 1, 220V AC输出CH1~CH3固定最大功率输出           */
    if( trun_flag == 0 )
    {
 
        AC_Out1 &= (ac_220.ac220_on_off & ~0x01); 
        AC_Out2 &= (ac_220.ac220_on_off & ~0x02); 
        AC_Out3 &= (ac_220.ac220_on_off & ~0x04); 
        trun_flag = 1;
        IE2  |=  0x00;				//关闭IE2-ET3，TIM3中断
    }
    if( trun_flag == 1 )
    {
        AC_Out1 = AC_Out2 = AC_Out3 = 0;
        trun_flag = 0;
    }
}