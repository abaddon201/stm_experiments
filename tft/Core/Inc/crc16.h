/*
 * crc16.h
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_CRC16_H_
#define INC_CRC16_H_

#include <stdio.h>

extern bool CRCReset;
extern uint32_t CRC16_Byte_Counter;
extern uint16_t CRC16_value;

extern void crc16(uint8_t NewData);
extern uint16_t crc16(uint8_t *pcBlock, uint16_t len);

#endif /* INC_CRC16_H_ */
