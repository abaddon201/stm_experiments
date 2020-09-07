/*
 * zx_tap_player.cpp
 *
 *  Created on: Sep 6, 2020
 *      Author: user
 */

#include "zx_tap_player.h"
#include <math.h>

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
