/*
 * zx_tap_player.cpp
 *
 *  Created on: Sep 6, 2020
 *      Author: user
 */

#include "zx_tap_player.h"
#include "zx_tap_feeder.h"
#include <math.h>
#include <stm32f4xx_hal.h>
#include "tim.h"
#include "dac.h"

namespace {
ZxTapPlayer* _player;
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
/*
 void ZxTapPlayer::outTone(unsigned char level, int samplesCount) {
 for (int n = 0; n < samplesCount; n++) {
 // FIXME: write to the DMA buffer
 //fwrite((unsigned char*) &level, 1, 1, hwav);
 }
 }

 void ZxTapPlayer::outByte(uint8_t byte) {
 for (int i = 0; i < 8; i++, byte <<= 1) {
 if (byte & 128) {
 outTone(HI, samplesCountOne);
 outTone(LOW, samplesCountOne);
 } else {
 outTone(HI, samplesCountZero);
 outTone(LOW, samplesCountZero);
 }
 }
 }
 */

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
/*
 bool ZxTapPlayer::writeCurrentSample() {
 if (currentSample.samples) {
 while (currentSample.samples != 0) {
 dmaBuffer[position] = currentSample.level;
 --currentSample.samples;
 position++;
 if (position >= TAP_DMA_BUFFER_SIZE / 2) {
 return false;
 }
 }
 }
 //prepare next sample
 return true;
 }
 */
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
    /*
     unsigned char byte;
     while (1) {
     unsigned short length;
     if (readedBytes - currentByte<2) {
     if (f_read(&file, fileBuff, FILE_BLOCK_SIZE, &readedBytes)!=FR_OK) {
     //FIXME: error handling
     break;
     }
     }
     //get block length
     length = (fileBuff[currentByte]<<8) | fileBuff[currentByte+1];
     currentByte+=2;
     //create pilot tone
     int pilotImpulses;

     if (fileBuff[currentByte]) {
     pilotImpulses = IMPULSNUMBER_PILOT_DATA;
     } else {
     pilotImpulses = IMPULSNUMBER_PILOT_HEADER;
     }
     for (int m = 0; m < pilotImpulses; m++) {
     outTone(HI, samplesCountPilot);
     outTone(LOW, samplesCountPilot);
     }
     //формируем синхросигнал
     outTone(HI, samplesCountSync1);
     outTone(LOW, samplesCountSync2);
     //читаем данные и выдаём их
     outByte(fileBuff[2]);
     for (int l = 1; l < length; l++) {
     unsigned char byte;
     //if (fread(&byte, 1, sizeof(char), file_tap) < sizeof(char))
     break;/
     outByte(byte);
     }
     outTone(HI, samplesCountSync3);
     //пауза
     outTone(LOW, samplesCountPause);
     }*/
}

void ZxTapPlayer::stop() {
    HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
    HAL_TIM_Base_Stop(&htim4);

}

ZxTapPlayer::ZxTapPlayer() {
}

