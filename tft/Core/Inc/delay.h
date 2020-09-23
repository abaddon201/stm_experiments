/*
 * delay.h
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include <cstdio>

extern void delay_us(uint32_t dly);

extern void delay_ms(volatile uint32_t micros);

#endif /* INC_DELAY_H_ */
