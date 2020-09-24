/*
 * config_file.cpp
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: Abby
 */

#include "../../_utils/Inc/config_file.h"

#include <string.h>

ConfigFile config;

const char* fileName = ".sys/config.bin";

bool ConfigFile::save() {
    FIL *MyFile = new FIL();
    FRESULT res; /* FatFs function common result code */
    memset(MyFile, 0, sizeof(FIL));

    res = f_open(MyFile, fileName, FA_WRITE);
    if (res != FR_OK) {
        Error_Handler();
        return false;
    }

    UINT bytesRead;
    res = f_write(MyFile, config, CONFIG_SIZE, &bytesRead);
    if ((bytesRead != CONFIG_SIZE) || (res != FR_OK)) {
        Error_Handler();
        return false;
    }
    return true;
}

bool ConfigFile::load() {
    FIL *MyFile = new FIL();
    FRESULT res; /* FatFs function common result code */
    memset(MyFile, 0, sizeof(FIL));

    res = f_open(MyFile, fileName, FA_READ);
    if (res != FR_OK) {
        Error_Handler();
        return false;
    }

    UINT bytesRead;
    res = f_read(MyFile, config, CONFIG_SIZE, &bytesRead);
    if ((bytesRead != CONFIG_SIZE) || (res != FR_OK)) {
        Error_Handler();
        return false;
    }
    return true;
}
