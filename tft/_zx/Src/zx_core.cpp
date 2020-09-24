/*
 * zx_core.cpp
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: Abby
 */

#include "../../_zx/Inc/zx_core.h"

#include <stm32f407xx.h>

void ZxCore::startCPUClock() {
    //Z80 tact
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    //RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM1->PSC = 1;
    // 0 - 7MHz
    // 1 - 3.5MHz
    // 3 - 1.75 MHz

    TIM1->ARR = 23;

    TIM1->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
    TIM1->BDTR |= TIM_BDTR_MOE;

    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

    TIM1->CR1 |= TIM_CR1_CEN;

    TIM1->CCR1 = 12;
    TIM1->CCR2 = 0;
}

void ZxCore::startAYClock() {
    //AY Tact
    RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    //RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM9->PSC = 3;
    // 0 - 7MHz
    // 1 - 3.5MHz
    // 3 - 1.75 MHz

    TIM9->ARR = 23;

    TIM9->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
    TIM9->BDTR |= TIM_BDTR_MOE;

    TIM9->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

    TIM9->CR1 |= TIM_CR1_CEN;

    TIM9->CCR1 = 12;
    TIM9->CCR2 = 0;
}

void ZxCore::startIntClock() {
    //Z80 int
    RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
    //RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM12->PSC = 839;
    // 0 - 7MHz
    // 1 - 3.5MHz
    // 3 - 1.75 MHz

    TIM12->ARR = 1999;

    TIM12->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
    TIM12->BDTR |= TIM_BDTR_MOE;

    TIM12->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

    TIM12->CR1 |= TIM_CR1_CEN;

    TIM12->CCR1 = 10;
    TIM12->CCR2 = 0;
}
