#ifndef _PWM_H_
#define _PWM_H_

#include "sys.h"

#define PWMx_7      7
#define PWMx_8      8

void PWM_Init( void );
void pwm8close( uint8_t pwm_channel );
void pwm8start( uint8_t pwm_channel );

#endif