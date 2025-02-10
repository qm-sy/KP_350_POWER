#include "common.h"

/**
 * @brief	一些定时执行的事件   Timer4：10us
 *
 * @param   
 *
 * @return  void
**/
void Tim4_ISR( void ) interrupt 20
{

    static uint16_t temp_scan_cnt = 0;
    
    /* 1, 220V AC输出CH1~CH3固定最大功率输出           */

    /* 2, temp 上报 1s/次                             */
    temp_scan_cnt++;
    if( temp_scan_cnt == 100 )
    {
        temp.temp_scan_flag = 1;
        temp_scan_cnt = 0;
    }
}