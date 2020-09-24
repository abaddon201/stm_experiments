/*
 * delay.cpp
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: Abby
 */

#include "../../_utils/Inc/delay.h"

#include <stm32f4xx.h>

void delay_us(uint32_t dly) {
    volatile uint32_t i;

    for (i = 0; i < dly; i++)
        ;
}

void delay_ms(volatile uint32_t micros) {
    micros *= (SystemCoreClock / 1000000) / 5;

    while (micros--)
        ;
}
