/*
 * zx_tap_player.cpp
 *
 *  Created on: Sep 6, 2020
 *      Author: user
 */

#include "zx_tap_player.h"
#include <math.h>
#include <stm32f4xx_hal.h>
#include "tim.h"
#include "dac.h"

ZxTapPlayer::ZxTapPlayer() {

}

static const uint32_t DAC_MAX_VAL = 0x00000FFFU;
static constexpr double PI = 3.1415926535897932384626433832795F;

void ZxTapPlayer::generateWave(uint16_t *dac_data, uint32_t dac_data_cnt, uint8_t duty) {
    uint32_t max_val = (DAC_MAX_VAL * duty) / 100U;
    uint32_t shift = (DAC_MAX_VAL - max_val) / 2U;
    for (uint32_t i = 0U; i < dac_data_cnt; i++) {
        dac_data[i] = (uint16_t) ((sin((2.0F * i * PI) / (dac_data_cnt + 1)) + 1.0F) * max_val) >> 1U;
        dac_data[i] += shift;
    }
}

#define NS  128
uint32_t Wave_LUT[NS] = {
    2048, 2149, 2250, 2350, 2450, 2549, 2646, 2742, 2837, 2929, 3020, 3108, 3193, 3275, 3355,
    3431, 3504, 3574, 3639, 3701, 3759, 3812, 3861, 3906, 3946, 3982, 4013, 4039, 4060, 4076,
    4087, 4094, 4095, 4091, 4082, 4069, 4050, 4026, 3998, 3965, 3927, 3884, 3837, 3786, 3730,
    3671, 3607, 3539, 3468, 3394, 3316, 3235, 3151, 3064, 2975, 2883, 2790, 2695, 2598, 2500,
    2400, 2300, 2199, 2098, 1997, 1896, 1795, 1695, 1595, 1497, 1400, 1305, 1212, 1120, 1031,
    944, 860, 779, 701, 627, 556, 488, 424, 365, 309, 258, 211, 168, 130, 97,
    69, 45, 26, 13, 4, 0, 1, 8, 19, 35, 56, 82, 113, 149, 189,
    234, 283, 336, 394, 456, 521, 591, 664, 740, 820, 902, 987, 1075, 1166, 1258,
    1353, 1449, 1546, 1645, 1745, 1845, 1946, 2047
};

constexpr int BASE_FREQ = 84000000/NS;

void ZxTapPlayer::playWave(uint32_t freq) {
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)Wave_LUT, 128, DAC_ALIGN_12B_R);
    /*RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    TIM4->PSC = 0;
    // 0 - 7MHz
    // 1 - 3.5MHz
    // 3 - 1.75 MHz

    TIM4->ARR = 624;

    //TIM12->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;
    //TIM12->BDTR |= TIM_BDTR_MOE;

    //TIM12->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;

    TIM4->CR1 |= TIM_CR1_CEN;
*/
    TIM4->PSC = 0;
    TIM4->ARR = BASE_FREQ/freq -1;
    HAL_TIM_Base_Start(&htim4);
}
