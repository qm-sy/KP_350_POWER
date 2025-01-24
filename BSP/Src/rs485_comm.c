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

        /* 3, 依次将TX2_buf中数据送出（写S2BUF操作即为发送）    */
        if( rs485.TX2_send_bytelength != 0 )
        {
            S2BUF = rs485.TX2_buf[rs485.TX2_send_cnt++];
            rs485.TX2_send_bytelength--;
        }
    }
    
    if( S2CON & S2RI )
    {
        S2CON &= ~S2RI;
        if( !rs485.RX2_rev_end_Flag )
        {
            if( rs485.RX2_rev_cnt > 128 )
            {
                rs485.RX2_rev_cnt = 0;
            }
            rs485.RX2_buf[rs485.RX2_rev_cnt] = S2BUF;
            rs485.RX2_rev_cnt++;
        }
        rs485.RX2_rev_timeout = 150;
    }
}

void Tim0_ISR( void ) interrupt 1   //1ms
{
    if ( rs485.RX2_rev_timeout != 0 )  //uart1中设定为50 开始执行
    {
        rs485.RX2_rev_timeout--;
        if( rs485.RX2_rev_timeout == 0 )  //超时
        {
            if( rs485.RX2_rev_cnt > 0 )  //接收的数据包不为空
            {
                rs485.RX2_rev_end_Flag = 1;   //接收完毕标志位亮起
                rs485.RX2_rev_cnt = 0;  //初始化接收缓冲区
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
static void Uart2_Sendbyte( uint8_t dat )
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
