/*
 * config_file.h
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_CONFIG_FILE_H_
#define INC_CONFIG_FILE_H_

#include <cstdio>

class ConfigFile {
public:
    static constexpr uint16_t TOUCH_PARAMS = 0;

    template<typename T>
    void put(int offset, T value) {

    }

    template<typename T>
    T get(int offset) {
        return (T) 0;
    }

    bool save();
};

extern ConfigFile config;

#endif /* INC_CONFIG_FILE_H_ */
