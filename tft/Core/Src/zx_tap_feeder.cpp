/*
 * zx_tap_feeder.cpp
 *
 *  Created on: 21 сент. 2020 г.
 *      Author: user
 */

#include "zx_tap_feeder.h"

#include <math.h>

namespace {
constexpr int IMPULSNUMBER_PILOT_HEADER = 8063;
constexpr int IMPULSNUMBER_PILOT_DATA = 3223;
constexpr int PULSE_LEN_PILOT = 2168;
constexpr int PULSE_LEN_SYNC1 = 667;
constexpr int PULSE_LEN_SYNC2 = 735;
constexpr int PULSE_LEN_SYNC3 = 954;
constexpr int PULSE_LEN_ZERO = 855;
constexpr int PULSE_LEN_ONE = 1710;
constexpr int PULSE_LEN_PAUSE = 2168 * 3223;
constexpr uint8_t HI = 255;
constexpr uint8_t LOW = 0;
}


bool ZxTapFeeder::nextSample(Sample &sample) {
    switch (state) {
    case State::START:
        if (readedBytes - currentByte < 3) {
            // read from 0 to unparsed block
            if (f_read(&file, fileBuff, FILE_BLOCK_SIZE - (readedBytes - currentByte), &readedBytes) != FR_OK) {
                //FIXME: error handling
                return false;
            }
            //readedBytes+=currentByte;
        }
        blockLength = getByte() | (getByte() << 8);
        // file type and first byte of the header
        byte = getByte();
        state = State::PILOT_ONE;
        sample.level = HI;
        sample.samples = samplesCountPilot;
        if (byte) {
            pilotLength = IMPULSNUMBER_PILOT_DATA;
        } else {
            pilotLength = IMPULSNUMBER_PILOT_HEADER;
        }
        break;
    case State::PILOT_ONE:
        state = State::PILOT_ZERO;
        sample.level = LOW;
        sample.samples = samplesCountPilot;
        break;
    case State::PILOT_ZERO:
        pilotLength--;
        if (pilotLength) {
            // continue pilot
            state = State::PILOT_ONE;
            sample.level = HI;
            sample.samples = samplesCountPilot;
        } else {
            // move to the sync1
            state = State::SYNC1;
            sample.level = HI;
            sample.samples = samplesCountSync1;
        }
        break;
    case State::SYNC1:
        state = State::SYNC2;
        sample.level = LOW;
        sample.samples = samplesCountSync2;
        break;
    case State::SYNC2:
        bitPosition=8;
        if (byte & 128) {
            state = State::ONE;
            sample.level = HI;
            sample.samples = samplesCountOne;
        } else {
            state = State::ZERO;
            sample.level = HI;
            sample.samples = samplesCountZero;
        }
        break;
    case State::ONE:
        sample.level = LOW;
        sample.samples = samplesCountOne;
        state=State::NEXT_BIT;
        break;
    case State::ZERO:
        sample.level = LOW;
        sample.samples = samplesCountZero;
        state=State::NEXT_BIT;
        break;
    case State::NEXT_BIT:
        encodeNextBit(sample);
        break;
    case State::SYNC3:
        state = State::START;
        sample.level = LOW;
        sample.samples = samplesCountPause;
        break;
    }

    return true;
}

void ZxTapFeeder::encodeNextBit(Sample& sample) {
    --bitPosition;
    if (bitPosition) {
        byte<<=1;
        if (byte & 128) {
            state = State::ONE;
            sample.level = HI;
            sample.samples = samplesCountOne;
        } else {
            state = State::ZERO;
            sample.level = HI;
            sample.samples = samplesCountZero;
        }
    } else {
        --blockLength;
        if (blockLength) {
            if (readedBytes - currentByte < 1) {
                // read from 0 to unparsed block
                if (f_read(&file, fileBuff, FILE_BLOCK_SIZE - (readedBytes - currentByte), &readedBytes) != FR_OK) {
                    //FIXME: error handling
                    return;
                }
                //readedBytes+=currentByte;
            }
            byte = getByte();
            bitPosition = 8;
            if (byte & 128) {
                state = State::ONE;
                sample.level = HI;
                sample.samples = samplesCountOne;
            } else {
                state = State::ZERO;
                sample.level = HI;
                sample.samples = samplesCountZero;
            }
        } else {
            //block ended
            state = State::SYNC3;
            sample.level = HI;
            sample.samples = samplesCountSync3;
        }
    }
}

int ZxTapFeeder::pulseLenToSamplesCount(int pulseLen) {
    // FIXME: looks like we can convert this math to ints
    double sampleNanosec = 1000000000 / freq;
    double cpuClkNanosec = 286;
    return round((cpuClkNanosec * double(pulseLen)) / sampleNanosec);
}

bool ZxTapFeeder::initFile(const char *fileName, int freq) {
    this->freq = freq;
    TIM4->PSC = 0;
    TIM4->ARR = 84000000 / freq - 1;
    if (f_open(&file, fileName, FA_READ)!=FR_OK) {
        //FIXME: handle errors
        return false;
    }
    return true;
}
