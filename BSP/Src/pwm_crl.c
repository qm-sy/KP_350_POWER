#include "pwm_crl.h"

/**
 * @brief	通过调节PWM占空比实现对应的风力大小
 *
 * @param   channel_num：pwm通道
 * @param   wind_num：风力大小
 *
 * @return  void
**/
void PWM_Crl( uint8_t pwm_channel, uint8_t wind_level )
{
    switch(pwm_channel)
    {
        case PWMx_7:
            PWMB_CCR7 = 184 * wind_level;
            break;
        case PWMx_8:
            PWMB_CCR8 = 184 * wind_level;
            break;      
        default:
            break;                          
    }
}
