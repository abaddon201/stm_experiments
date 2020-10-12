/*
 * eeprom.cpp
 *
 *  Created on: 25 сент. 2020 г.
 *      Author: Abby
 */

#include "eeprom.h"
#include "delay.h"
#include "base.h"
#include "fatfs.h"
#include "fs_utils.h"

#include "zx_core.h"

#include <malloc.h>
#include <stm32f407xx.h>

#define PAGE_SIZE 128

void Eeprom::setAddress(int addr) {
    uint16_t lowAddr = addr & 16383;
    uint16_t highAddr = addr >> 13;
    uint16_t rom_reg = ROM_CTRL->IDR;
    rom_reg &= 0b1111111111111000;
    ROM_CTRL->ODR = rom_reg | highAddr;
    REG_A->ODR = lowAddr;
}

void Eeprom::writeByte(uint8_t data) {
    REG_D->ODR = data;
}

void Eeprom::pulseWE() {
    ROM_CTRL->BSRR = ROM_WE << 16;   // reset
    delay_ms(1);
    ROM_CTRL->BSRR = ROM_WE;   // set
    delay_ms(1);
}

void Eeprom::pulseOE() {
    ROM_CTRL->BSRR = ROM_OE << 16;   // reset
    delay_ms(1);
    ROM_CTRL->BSRR = ROM_OE;   // set
    delay_ms(1);
}

void Eeprom::OELow() {
    ROM_CTRL->BSRR = ROM_OE << 16;   // reset
}

void Eeprom::OEHigh() {
    ROM_CTRL->BSRR = ROM_OE;   // set
}

bool Eeprom::writePage(uint8_t *buffer, int offset) {
    setAddress(0x5555);
    writeByte(0xAA);
    pulseWE();

    setAddress(0x2AAA);
    writeByte(0x55);
    pulseWE();

    setAddress(0x5555);
    writeByte(0xA0);
    pulseWE();

    uint8_t lastByte;
    for (long j = 0; j < PAGE_SIZE; j++) {
        setAddress(j);
        lastByte = buffer[j];
        writeByte(lastByte);
        delay_ms(5);
        pulseWE();
        delay_ms(10);
    }
    delay_us(11);
    //check that async write is finished
    uint8_t reg;
    do {
        OELow();
        reg = REG_D->IDR;
        OEHigh();
    } while (reg != lastByte);
    return true;
}

bool Eeprom::write(uint8_t *buffer, int offset, int size) {
    ROM_CTRL->BSRR = ROM_CE << 16;   // reset
    int pages = size / PAGE_SIZE;
    for (int i = 0; i < pages; i++) {
        bool res = writePage(&buffer[i * PAGE_SIZE], offset + i * PAGE_SIZE);
        if (!res) {
            baseErrorHandler("Error writing EEPROM");
            ROM_CTRL->BSRR = ROM_CE;   // set
            return false;
        }
    }
    ROM_CTRL->BSRR = ROM_CE;   // set
    return true;
}

bool Eeprom::writeFromFile(const char *fileName, int offset, int size) {
    uint8_t *buffer = (uint8_t*) malloc(PAGE_SIZE);
    FIL *file = (FIL*) malloc(sizeof(FIL));
    FRESULT ret = f_open(file, fileName, FA_READ);
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
        free(file);
        free(buffer);
        return false;
    }

    ROM_CTRL->BSRR = ROM_CE << 16;   // reset
    int pages = size / PAGE_SIZE;
    UINT br;
    for (int i = 0; i < pages; i++) {
        ret = f_read(file, buffer, PAGE_SIZE, &br);
        if (ret) {
            baseErrorHandler(fileSystemFault(ret));
            free(file);
            free(buffer);
            return false;
        }

        bool res = writePage(buffer, offset + i * PAGE_SIZE);
        if (!res) {
            baseErrorHandler("Error writing EEPROM");
            ROM_CTRL->BSRR = ROM_CE;   // set
            free(file);
            free(buffer);
            return false;
        }
    }
    ROM_CTRL->BSRR = ROM_CE;   // set
    free(file);
    free(buffer);
    return true;
}
