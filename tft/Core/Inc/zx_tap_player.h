/*
 * zx_tap_player.h
 *
 *  Created on: Sep 6, 2020
 *      Author: user
 */

#ifndef INC_ZX_TAP_PLAYER_H_
#define INC_ZX_TAP_PLAYER_H_

#include <stdio.h>

#define TAP_DMA_BUFFER_SIZE 8192

#include "zx_tap_sample.h"

class ZxTapFeeder;
class ZxTapPlayer {

    // FIXME: define size according to the speed and memory available
    uint8_t dmaBuffer[TAP_DMA_BUFFER_SIZE];
    bool fileEnded;

    bool writeCurrentSample();

    ZxTapFeeder *feeder;
    Sample currentSample;

public:
    ZxTapPlayer();

    /**
     * play tap file
     */
    void play(ZxTapFeeder *feeder);

    /**
     * pause playing file
     */
    void pause();

    void fillBuffer(int part);
    /**
     * stop playing file
     */
    void stop();
};

#endif /* INC_ZX_TAP_PLAYER_H_ */
