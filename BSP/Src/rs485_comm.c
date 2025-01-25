#include "rs485_comm.h"

RS485 rs485;

/**
 * @brief	串口2中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Uart2_ISR() interrupt 8 
{   
    /* 1, 检测到硬件将S2TI置1，即发送完毕                       */
    if( S2CON & S2TI )          //
    {
        /* 2, 软件将S2TI清零，等待发送标志位重置，可继续发送    */
        S2CON &= ~S2TI;         
        rs485.TX2_busy_Flag = 0;

        // /* 3, 依次将TX2_buf中数据送出（写S2BUF操作即为发送）    */
        // if( rs485.TX2_send_bytelength != 0 )
        // {
        //     S2BUF = rs485.TX2_buf[rs485.TX2_send_cnt++];
        //     rs485.TX2_send_bytelength--;
        // }
    }
    
    /* 1, 检测到硬件将S2RI置1，即接收完毕                       */
    if( S2CON & S2RI )
    {
        /* 2, 软件将S2RI清零，等待接收标志位重置，可继续发送    */
        S2CON &= ~S2RI;

        /* 3, 判断数据包是否接收完毕                           */
        if( !rs485.RX2_rev_end_Flag )
        {
            /* 4, 数据包大于RX_buf 则从头计数                  */
            if( rs485.RX2_rev_cnt > 128 )
            {
                rs485.RX2_rev_cnt = 0;
            }

            /* 5, 依次将RX2_buf中数据接收（读S2BUF操作即为接收）*/
            rs485.RX2_buf[rs485.RX2_rev_cnt] = S2BUF;
            rs485.RX2_rev_cnt++;
        }
        /* 6, 重置接收完毕判断时间                              */
        rs485.RX2_rev_timeout = 50;
    }
}

/**
 * @brief	是否接收完毕判断函数
 *
 * @param   
 *
 * @return  void
**/
void Tim0_ISR( void ) interrupt 1   //1ms
{
    /* 1, 如果接收未超时                                             */
    if ( rs485.RX2_rev_timeout != 0 )  
    {
        rs485.RX2_rev_timeout--;
        /* 2, 如果接收超时                                          */
        if( rs485.RX2_rev_timeout == 0 )  
        {
            if( rs485.RX2_rev_cnt > 0 )  
            {   
                 /* 3, 接收完毕标志位亮起并初始化接收缓冲区         */
                rs485.RX2_rev_end_Flag = 1;   
                rs485.RX2_rev_cnt = 0;  
            }
        }
    } 
}

/**
 * @brief	串口2发送1字节数据
 *
 * @param   
 *
 * @return  void
**/
void Uart2_Sendbyte( uint8_t dat )
{
    while( rs485.TX2_busy_Flag );
    rs485.TX2_busy_Flag = 1;
    S2BUF = dat;
}

/**
 * @brief	串口2发送字符串
 *
 * @param   
 *
 * @return  void
**/
void Uart2_SendStr( uint8_t *sendstr )
{   
    while(*sendstr)
    {
        Uart2_Sendbyte( *sendstr++ );
    }
}

/**
 * @brief	不定长数据接收测试函数
 *
 * @param   
 *
 * @return  void
**/
void uart2_test( void )
{
    if( rs485.RX2_rev_end_Flag == 1 )
    {
        if( rs485.RX2_buf[0]== 0x58 )
        {
            if ( rs485.RX2_buf[1] == 0x85 )
            {
                Uart2_SendStr("receive success \r\n");
            }else
            {
                Uart2_SendStr("receive error \r\n");
            }
            
        }
    }
}
