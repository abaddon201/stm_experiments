/*
 * zx_tap_player.h
 *
 *  Created on: Sep 6, 2020
 *      Author: user
 */

#ifndef INC_ZX_TAP_PLAYER_H_
#define INC_ZX_TAP_PLAYER_H_

#include <stdio.h>

class ZxTapPlayer {
    void generateWave(uint16_t *dac_data, uint32_t dac_data_cnt, uint8_t duty);

public:
    ZxTapPlayer();

    void playWave(uint32_t freq);
};

#endif /* INC_ZX_TAP_PLAYER_H_ */
