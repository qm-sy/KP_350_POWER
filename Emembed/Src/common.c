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
    static uint8_t trun_flag = 1;
    static uint16_t temp_scan_cnt1 = 0;
    static uint16_t temp_scan_cnt2 = 0;
    /* 1, 220V AC输出CH1~CH3固定最大功率输出           */
    if( trun_flag == 0)
    {
        AC_Out1 = AC_Out2 = AC_Out3 = 1;
    }
    if(( ac_220.zero_flag == 1 ) && ( trun_flag == 1))
    {
        AC_Out1 &= (ac_220.ac220_on_off & ~0x01); 
        AC_Out2 &= (ac_220.ac220_on_off & ~0x02); 
        AC_Out3 &= (ac_220.ac220_on_off & ~0x04); 
        trun_flag = 0;
    }
    if( ac_220.zero_flag == 0 )
    {
        trun_flag = 1;
    }

    /* 2, temp 上报 1s/次                             */
    temp_scan_cnt1++;
    if( temp_scan_cnt1 == 100 )
    {
        temp_scan_cnt1 = 0;
        temp_scan_cnt2++;
        if( temp_scan_cnt2 == 1000 )
        {
            temp.temp_scan_flag = 1;
            temp_scan_cnt2 = 0;
        }
    }
}