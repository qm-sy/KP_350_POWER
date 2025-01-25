#include "main.h"

void main( void )
{
    P_SW2 |= 0x80;
    GPIO_Init();
    ADC_Init();
    Uart2_Init();
    EA = 1;
    delay_ms(1);
    delay_us(1);
    Get_ADC_12bit(0);
    get_temp(1);
    PWM_Crl(7,3);
    
    Uart2_SendStr( "========== code start ==========" );
    
    while (1)
    {
        uart2_test();
    }
    
    
}