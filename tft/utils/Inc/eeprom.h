/*
 * eeprom.h
 *
 *  Created on: 25 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include <stdio.h>

class Eeprom {
    void setAddress(int addr);
    void writeByte(uint8_t data);
    void pulseWE();
    void pulseOE();
    bool writePage(uint8_t* buffer, int offset);
public:
    bool write(uint8_t* buffer, int offset, int size);
    bool writeFromFile(const char* fileName, int offset, int size);
};



#endif /* INC_EEPROM_H_ */
