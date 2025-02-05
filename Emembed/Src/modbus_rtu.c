#include "modbus_rtu.h"

/**
 * @brief	modbus_rtu  无奇偶校验
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Event( void )
{
    uint16_t crc,rccrc;

    /*1.接收完毕                                           */
    if( rs485.RX2_rev_end_Flag == 1 )
    {
        /*2.清空接收完毕标志位                              */    
        rs485.RX2_rev_end_Flag = 0;

        /*3.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.RX2_buf, rs485.RX2_rev_cnt-2);
        rccrc = (rs485.RX2_buf[rs485.RX2_rev_cnt-2]<<8) | (rs485.RX2_buf[rs485.RX2_rev_cnt-1]);

        /*4.清空接收计数                                    */
        rs485.RX2_rev_cnt = 0; 

        /*5.CRC校验通过，进行地址域校验                      */
        if( crc == rccrc )
        {
            /*6.地址域校验通过，进入相应功能函数进行处理      */
            if( rs485.RX2_buf[0] == MY_ADDR )
            {
                switch ( rs485.RX2_buf[1] )
                {
                    case 3:
                        Modbus_Fun3();
                        break;
                    case 4:
                        Modbus_Fun4();
                        break;
                    case 6:
                        Modbus_Fun6();
                        break;                           
                    default:
                        break;
                }
            }
        }
    }
}

/**
 * @brief	读输出寄存器  03
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun3( void )
{
    uint8_t info_byte = 0;

    switch (rs485.RX2_buf[3])
    {
        /* 40001   两路PWM 开关状态及风速查询                 */
        case 0:
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = 2;                       //Byte Count

            info_byte |= (PWMB_CCR7 / 184)<<2;          //PWM7风速
            info_byte |= (PWMB_CCR8 / 184)<<5;          //PWM8风速
            if( PWMB_CCER2 & 0X01 )
            {
                info_byte |= 0x01;                      //PWM7开关状态
            }
            if( PWMB_CCER2 & 0X10 )
            {
                info_byte |= 0x02;                      //PWM8开关状态
            }

            rs485.TX2_buf[3] = 0x00;                    //Data1 H
            rs485.TX2_buf[4] = info_byte;               //Data1 L

            slave_to_master(5);

            break;

        /* 40002   24V LED开关状态查询                       */
        case 1:
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = 2;                       //Byte Count

            if( EX0 & 1 )
            {
                info_byte |= 0x01;                      //LED开关状态
            }
            rs485.TX2_buf[3] = 0x00;                    //Data1 H
            rs485.TX2_buf[4] = info_byte;               //Data1 L

            slave_to_master(5);

            break;

        /* 40003   220V 输出状态查询                            */
        case 2 :
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = 2;                       //Byte Count

            info_byte = ((ac_220.time_delay - 58000) / 75)<<1;
            if( EX0 & 1 )
            {
                info_byte |= 0x01;                      //220运行状态
            }

            rs485.TX2_buf[3] = 0x00;                    //Data1 H
            rs485.TX2_buf[4] = info_byte;              //Data1 L

            slave_to_master(5);

            break;

        /* 40004 && 40005   温度报警值                      */
        case 3 :
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = 4;                       //Byte Count

            rs485.TX2_buf[3] = temp.power_ch2_temp_alarm;   //Data1 H
            rs485.TX2_buf[4] = temp.power_ch2_temp_alarm;   //Data1 L
            rs485.TX2_buf[5] = 0x00;                        //Data2 H
            rs485.TX2_buf[6] = temp.power_ch3_temp_alarm;   //Data2 L

            slave_to_master(7);

            break;

        default:
            break; 
    }
}

/**
 * @brief	读输入寄存器  04
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun4( void )
{
    switch (rs485.RX2_buf[3])
    {
        /*    30001 && 30002 4路NTC                       */
        case 0:                                         
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = 4;                       //Byte Count
            rs485.RX2_buf[3] = get_temp(NTC_2);
            rs485.RX2_buf[4] = get_temp(NTC_1);
            rs485.RX2_buf[5] = get_temp(NTC_4);
            rs485.RX2_buf[6] = get_temp(NTC_3);

            slave_to_master(7);

            break;

        /*    30003 2路模拟量                               */
        case 2:    
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = 2;                       //Byte Count

            rs485.TX2_buf[3] = 0xaa;                    //Data1 H
            rs485.TX2_buf[4] = 0xbb;                    //Data1 L

            slave_to_master(5);

            break;

        /*    30003 3路电流查询                             */
        case 3:    
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = 4;                       //Byte Count

            rs485.TX2_buf[3] = get_current(I_OUT2);     //Data1 H
            rs485.TX2_buf[4] = get_current(I_OUT1);     //Data1 L
            rs485.TX2_buf[5] = 0x00;                    //Data2 H
            rs485.TX2_buf[6] = get_current(I_OUT3);     //Data2 L

            slave_to_master(5);

            break;

        default:
            break;
    }
}

/**
 * @brief	写单个输出寄存器  06
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun6( void )
{
    switch (rs485.RX2_buf[3])
    {
        /*  40001  两路PWM 开关状态及风速设置                 */
        case 0:                                         
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = rs485.RX2_buf[2];        //reg H
            rs485.TX2_buf[3] = rs485.RX2_buf[3];        //reg L
            rs485.TX2_buf[4] = rs485.RX2_buf[4];        //Value H
            rs485.TX2_buf[5] = rs485.RX2_buf[5];        //Value L
            rs485.TX2_buf[6] = rs485.RX2_buf[6];        //CRC H
            rs485.TX2_buf[7] = rs485.RX2_buf[7];        //CRC L

            if( rs485.TX2_buf[5] & 0X01 )
            {
                PWMB_CCER2 |= 0X01;
            }else
            {
                PWMB_CCER2 &= 0XFE;
            }
            if( rs485.TX2_buf[5] & 0X02 )
            {
                PWMB_CCER2 |= 0X10;
            }else
            {
                PWMB_CCER2 &= 0XEF;
            }
            
            PWMB_CCR7 = ((rs485.TX2_buf[5]>>2) & 0x07)*184;
            PWMB_CCR8 = (rs485.TX2_buf[5]>>5)*184;

            rs485.TX2_send_bytelength = 8;

            S2CON |= S2TI;                              //开始发送

            eeprom.pwm_info = rs485.TX2_buf[5];
            eeprom_data_record();

          break;

        /*  40002  24V LED开关状态设置                          */
        case 1:                                         
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = rs485.RX2_buf[2];        //reg H
            rs485.TX2_buf[3] = rs485.RX2_buf[3];        //reg L
            rs485.TX2_buf[4] = rs485.RX2_buf[4];        //Value H
            rs485.TX2_buf[5] = rs485.RX2_buf[5];        //Value L
            rs485.TX2_buf[6] = rs485.RX2_buf[6];        //CRC H
            rs485.TX2_buf[7] = rs485.RX2_buf[7];        //CRC L

            if( rs485.TX2_buf[5] & 0X01 )
            {
                DC_24V_out(1);
            }else
            {
                DC_24V_out(0);
            }
            
            rs485.TX2_send_bytelength = 8;

            S2CON |= S2TI;                              //开始发送

            eeprom.led_info = rs485.TX2_buf[5];
            eeprom_data_record();

            break;

        /*  40003  220V 开关及大小设置                          */
        case 2:                                         
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = rs485.RX2_buf[2];        //reg H
            rs485.TX2_buf[3] = rs485.RX2_buf[3];        //reg L
            rs485.TX2_buf[4] = rs485.RX2_buf[4];        //Value H
            rs485.TX2_buf[5] = rs485.RX2_buf[5];        //Value L
            rs485.TX2_buf[6] = rs485.RX2_buf[6];        //CRC H
            rs485.TX2_buf[7] = rs485.RX2_buf[7];        //CRC L

            if( rs485.TX2_buf[5] & 0X01 )
            {
                EX0 = 1;
            }else
            {
                EX0 = 0;
            }
            AC_220V_out(rs485.TX2_buf[5]>>1);

            rs485.TX2_send_bytelength = 8;

            S2CON |= S2TI;                              //开始发送

            eeprom.ac220_info = rs485.TX2_buf[5];
            eeprom_data_record();

            break;  

        default:
            break;   
    }
}

/**
 * @brief	写多个输出寄存器  16
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun16( void )
{
    uint16_t crc;

    switch (rs485.RX2_buf[3])
    {
        case 3:
            rs485.TX2_buf[0] = rs485.RX2_buf[0];        //地址域
            rs485.TX2_buf[1] = rs485.RX2_buf[1];        //功能域
            rs485.TX2_buf[2] = rs485.RX2_buf[2];        //start reg H
            rs485.TX2_buf[3] = rs485.RX2_buf[3];        //start reg L
            rs485.TX2_buf[4] = rs485.RX2_buf[4];        //Num H
            rs485.TX2_buf[5] = rs485.RX2_buf[5];        //Num L

            temp.temp1_alarm_value = rs485.RX2_buf[9];
            temp.temp2_alarm_value = rs485.RX2_buf[8];
            temp.temp3_alarm_value = rs485.RX2_buf[11];

            crc = MODBUS_CRC16(rs485.TX2_buf,6);

            rs485.TX2_buf[6] = crc>>8;                 //CRC H
            rs485.TX2_buf[7] = crc;                  //CRC L

            rs485.TX2_send_bytelength = 8;

            S2CON |= S2TI;                                  //开始发送

            eeprom.temp_alarm_value1 = temp.temp1_alarm_value;
            eeprom.temp_alarm_value2 = temp.temp2_alarm_value;
            eeprom.temp_alarm_value3 = temp.temp3_alarm_value;

            eeprom_data_record();

        break;
    }

}
/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length)
{
	uint8_t	i;
	uint16_t	crc16;

    /* 1, 预置16位CRC寄存器为0xffff（即全为1）                          */
	crc16 = 0xffff;	

	do
	{
        /* 2, 把8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器     */        
		crc16 ^= (uint16_t)*buf;		//
		for(i=0; i<8; i++)		
		{
            /* 3, 如果最低位为1，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位 再异或0xA001    */
			if(crc16 & 1)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            /* 4, 如果最低位为0，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位                */
            else
            {
                crc16 >>= 1;
            }		
		}
		buf++;
	}while(--length != 0);

	return	(crc16);
}

/**
 * @brief	从机回复主机
 *  
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
void slave_to_master(uint8_t length)
{
    uint16_t crc;

    crc = MODBUS_CRC16(rs485.TX2_buf,length);

    rs485.TX2_buf[length] = crc>>8;                 //CRC H
    rs485.TX2_buf[length+1] = crc;                  //CRC L

    rs485.TX2_send_bytelength = length + 2;

    S2CON |= S2TI;                                  //开始发送
}