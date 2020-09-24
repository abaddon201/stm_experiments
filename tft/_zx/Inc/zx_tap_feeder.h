/*
 * zx_tap_feeder.h
 *
 *  Created on: 21 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_ZX_TAP_FEEDER_H_
#define INC_ZX_TAP_FEEDER_H_

#include "ff.h"

#include "../../_zx/Inc/zx_tap_sample.h"

class ZxTapFeeder {
    static constexpr int FILE_BLOCK_SIZE = 1024;
    int freq = 44100;
    int samplesCountPilot = 27;
    int samplesCountSync1 = 8;
    int samplesCountSync2 = 9;
    int samplesCountSync3 = 12;
    int samplesCountZero = 11;
    int samplesCountOne = 22;
    int samplesCountPause = 88133;

    FIL file;

    UINT readedBytes = 0;
    UINT currentByte = 0;
    uint8_t fileBuff[FILE_BLOCK_SIZE];
    uint16_t blockLength;

    //current byte for output
    uint8_t byte;
    int bitPosition;
    int pilotLength;

    enum class State {
        START, PILOT_ONE, PILOT_ZERO, SYNC1, SYNC2, ONE, ZERO, SYNC3, NEXT_BIT
    };

    State state;
    inline uint8_t getByte() {
        uint8_t res = fileBuff[currentByte];
        currentByte = (currentByte + 1) % FILE_BLOCK_SIZE;
        return res;
    }
    void encodeNextBit(Sample &sample);

    int pulseLenToSamplesCount(int pulseLen);

public:
    /**
     * preload file part and tune constants for desired sampling frequency
     */
    bool initFile(const char *fileName, int freq);
    bool nextSample(Sample &sample);

};

#endif /* INC_ZX_TAP_FEEDER_H_ */
