#include "common.h"

/**
 * @brief	一些定时执行的事件   Timer3：10ms
 *
 * @param   
 *
 * @return  void
**/
void Tim3_ISR( void ) interrupt 19 
{
    /* 1, 100ms内如果无外部中断产生，则说明无220V输入   */
    ac_220.AC_Statu_cnt--;
    if( ac_220.AC_Statu_cnt == 0)
    {
        ac_220.AC_Statu = 0;
    }else
    {
        ac_220.AC_Statu = 1;
    }
}