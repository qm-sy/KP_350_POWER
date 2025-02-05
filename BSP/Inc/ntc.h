#ifndef _NTC_H_
#define _NTC_H_

#include "sys.h"
#include "adc.h"
#include "power_crl.h"

#define NTC_1        5
#define NTC_2        6
#define NTC_3        7
#define NTC_4        8

typedef struct 
{
    uint8_t  temp1_value;   
    uint8_t  temp2_value;  
    uint8_t  temp3_value;  
    uint8_t  temp1_alarm_value;
    uint8_t  temp2_alarm_value;          
    uint8_t  temp3_alarm_value;    
    uint8_t  temp_scan_flag;
}TEMP;

extern TEMP temp;

uint16_t get_temp( uint8_t channle_x );
void temp_scan( void );

#endif