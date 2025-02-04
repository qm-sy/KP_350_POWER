#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "communication.h"
#include "ntc.h"
#include "gpio.h"
#include "power_crl.h"
#include "cc6903.h"

#define MY_ADDR     0x35

void Modbus_Event( void );
void Modbus_Fun3( void );
void Modbus_Fun4( void );
void Modbus_Fun6( void );
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);


#endif