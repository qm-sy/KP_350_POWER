#include "ntc.h"

#define NTCTabNum 121

TEMP temp;

static uint16_t MF52E_tab[NTCTabNum]={
    282,/* 0 */
    291,/* 1 */     300,/* 2 */     309,/* 3 */     318,/* 3 */     327,/* 5 */
    337,/* 6 */     346,/* 7 */     356,/* 8 */     366,/* 9 */     375,/* 10 */
    385,/* 11 */    395,/* 12 */    404,/* 13 */    414,/* 14 */    424,/* 15 */
    434,/* 16 */    444,/* 17 */    454,/* 18 */    463,/* 19 */    473,/* 20 */
    483,/* 21 */    493,/* 22 */    502,/* 23 */    512,/* 24 */    522,/* 25 */
    531,/* 26 */    541,/* 27 */    550,/* 28 */    559,/* 29 */    568,/* 30 */
    578,/* 31 */    587,/* 32 */    596,/* 33 */    604,/* 34 */    613,/* 35 */
    622,/* 36 */    630,/* 37 */    639,/* 38 */    647,/* 39 */    655,/* 40 */
    663,/* 41 */    671,/* 42 */    679,/* 43 */    687,/* 44 */    694,/* 45 */
    702,/* 46 */    709,/* 47 */    716,/* 48 */    723,/* 49 */    730,/* 50 */
    737,/* 51 */    743,/* 52 */    750,/* 53 */    756,/* 54 */    763,/* 55 */
    769,/* 56 */    775,/* 57 */    781,/* 58 */    786,/* 59 */    792,/* 60 */
    797,/* 61 */    803,/* 62 */    808,/* 63 */    813,/* 64 */    818,/* 65 */
    823,/* 66 */    828,/* 67 */    833,/* 68 */    837,/* 69 */    842,/* 70 */
    846,/* 71 */    850,/* 72 */    854,/* 73 */    858,/* 74 */    862,/* 75 */
    866,/* 76 */    870,/* 77 */    874,/* 78 */    877,/* 79 */    881,/* 80 */
    884,/* 81 */    888,/* 82 */    891,/* 83 */    894,/* 84 */    897,/* 85 */
    900,/* 86 */    903,/* 87 */    906,/* 88 */    909,/* 89 */    911,/* 90 */
    914,/* 91 */    917,/* 92 */    919,/* 93 */    921,/* 94 */    924,/* 95 */
    926,/* 96 */    928,/* 97 */    931,/* 98 */    933,/* 99 */    935,/* 100 */
    937,/* 101 */   939,/* 102 */   941,/* 103 */   943,/* 104 */   945,/* 105 */
    946,/* 106 */   948,/* 107 */   950,/* 108 */   952,/* 109 */   953,/* 110 */
    955,/* 111 */   956,/* 112 */   958,/* 113 */   959,/* 114 */   961,/* 115 */
    962,/* 116 */   964,/* 117 */   965,/* 118 */   966,/* 119 */   967 /* 120 */
};	

/**
 * @brief 查表 找到adc_val对应的temp
 *
 * @param[in] temp_tab 对照表
 * @param[in] tab_num 表元素个数
 * @param[in] adc_val adc_val
 * 
 * @return  temp
 * 
**/
static uint16_t LookupTable( uint16_t *temp_tab, uint8_t tab_num, uint16_t adc_val )
{
		uint8_t	i;
        uint16_t temp_val;
	
		for( i = 0; i < (tab_num-1); i++ )
		{
				if(( adc_val >= temp_tab[i] ) && ( adc_val < temp_tab[i+1]) )
				temp_val = i;	
		}
		return temp_val;
}

/**
 * @brief 取10次adc_val均值对应的temp
 *
 * @param[in] channle_x ntc通道1~4
 * 
 * @return  temp :最终温度
 * 
**/
uint16_t get_temp( uint8_t channle_x )
{
    uint8_t t;
    uint16_t adc_val;
    uint16_t temp;

    /* 1, 取10次adc_val                           */
    for( t = 0; t < 10; t++ )
		{
			adc_val += Get_ADC_12bit(channle_x);
		}	       
    adc_val = adc_val/10;

    /* 2, 查表，找到对应温度值                     */
    temp = LookupTable( MF52E_tab, NTCTabNum, adc_val );

    return temp;
}

/**
 * @brief 温度扫描，1s/次 控制功率开关
 *
 * @param[in] 
 * 
 * @return  
 * 
**/
void temp_scan( void )
{
    if( temp.temp_scan_flag == 1)
    {
        temp.temp_value1 =  get_temp(NTC_1);
        temp.temp_value2 =  get_temp(NTC_2);
        temp.temp_value3 =  get_temp(NTC_3);

        if( temp.temp_value1 >= temp.temp_alarm_value1 )
        {
            ac_220.ac220_on_off &= ~0x01;        //001
        }else
        {
            ac_220.ac220_on_off |= 0x01;
        }

        if( temp.temp_value2 >= temp.temp_alarm_value2 )
        {
            ac_220.ac220_on_off &= ~0x02;        //101
        }else
        {
            ac_220.ac220_on_off |= 0x02;
        }

        if( temp.temp_value3 >= temp.temp_alarm_value3 )
        {
            ac_220.ac220_on_off &= ~0x04;        //011
        }else
        {
            ac_220.ac220_on_off |= 0x04;
        }

        temp.temp_scan_flag = 0;
    }
}