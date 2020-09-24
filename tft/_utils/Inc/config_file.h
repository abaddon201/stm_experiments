/*
 * config_file.h
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_CONFIG_FILE_H_
#define INC_CONFIG_FILE_H_

#include <fatfs.h>
#include <cstdio>

class ConfigFile {
    static constexpr int CONFIG_SIZE = 128;
    uint8_t config[CONFIG_SIZE];
public:
    static constexpr uint16_t TOUCH_PARAMS = 0;

    template<typename T>
    void put(int offset, T value) {
        uint8_t *val = (uint8_t*) &value;
        for (size_t i = 0; i < sizeof(T); ++i) {
            config[offset + i] = val[i];
        }
    }

    template<typename T>
    T get(int offset) {
        T value;
        uint8_t *val = (uint8_t*) &value;
        for (size_t i = 0; i < sizeof(T); ++i) {
            val[i] = config[offset + i];
        }

        return value;
    }

    bool save();

    bool load();
};

extern ConfigFile config;

#endif /* INC_CONFIG_FILE_H_ */
