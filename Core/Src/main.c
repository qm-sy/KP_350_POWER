#include "main.h"

void main( void )
{
    P_SW2 |= 0x80;
    GPIO_Init();
    Uart4_Init();
    /*  温度控制  */
    ADC_Init();
    
    /*  485控制  */
    Uart2_Init();
    Uart2_Send_Statu_Init();
    Timer0_Init();

    /*  4路220输出控制  */
    INT0_Init();
    Timer1_Init();
    Timer3_Init();
    Power_Statu_Init();

    /*  PWM控制  */
    PWM_Init();

    EA = 1;
   
    eeprom_test();

    printf( "========== code start ==========" );
    while (1)
    {
        Modbus_Event();
    }  
}