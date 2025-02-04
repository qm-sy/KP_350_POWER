#ifndef _ADC_H_
#define _ADC_H_

#include "sys.h"
#include "delay.h"
#include "stdio.h"

#define NTC_1        5
#define NTC_2        6
#define NTC_3        7
#define NTC_4        8
        
#define I_OUT1      10
#define I_OUT2      11
#define I_OUT3      12

void ADC_Init( void );
uint16_t Get_ADC_12bit( uint8_t ADC_Channel )	;

#endif