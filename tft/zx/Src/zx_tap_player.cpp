/*
 * zx_tap_player.cpp
 *
 *  Created on: Sep 6, 2020
 *      Author: Abby
 */

#include "zx_tap_player.h"

#include <math.h>
#include <stm32f4xx_hal.h>
#include "tim.h"
#include "dac.h"

#include "zx_tap_feeder.h"

namespace {
ZxTapPlayer *_player;
}

extern "C" {
/**
 * @brief  Conversion complete callback in non blocking mode for Channel1
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
    /* Prevent unused argument(s) compilation warning */
    _player->fillBuffer(1);
}

/**
 * @brief  Conversion half DMA transfer callback in non blocking mode for Channel1
 * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
 *         the configuration information for the specified DAC.
 * @retval None
 */
void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
    /* Prevent unused argument(s) compilation warning */
    _player->fillBuffer(0);
}

}

void ZxTapPlayer::fillBuffer(int part) {
    if (fileEnded) {
        stop();
        return;
    }
    uint8_t *buffer = &(dmaBuffer[part * (TAP_DMA_BUFFER_SIZE / 2)]);
    int position = 0;
    if (currentSample.samples) {
        while (currentSample.samples != 0) {
            buffer[position] = currentSample.level;
            --currentSample.samples;
            position++;
            if (position >= TAP_DMA_BUFFER_SIZE / 2) {
                return;
            }
        }
    }
    //prepare next sample
    while (feeder->nextSample(currentSample)) {
        while (currentSample.samples != 0) {
            buffer[position] = currentSample.level;
            --currentSample.samples;
            position++;
            if (position >= TAP_DMA_BUFFER_SIZE / 2) {
                return;
            }
        }
    }
    // file ended, need to fill with silence, and set flag to stop next time
    fileEnded = true;
    for (; position < TAP_DMA_BUFFER_SIZE / 2; ++position) {
        buffer[position] = 0;
    }
}

void ZxTapPlayer::play(ZxTapFeeder *feeder) {
    _player = this;
    this->feeder = feeder;
    fileEnded = false;
    fillBuffer(0);
    fillBuffer(1);

    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) dmaBuffer, TAP_DMA_BUFFER_SIZE, DAC_ALIGN_8B_R);
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
    HAL_TIM_Base_Start(&htim4);
}

void ZxTapPlayer::stop() {
    HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
    HAL_TIM_Base_Stop(&htim4);
}

ZxTapPlayer::ZxTapPlayer() {
}
