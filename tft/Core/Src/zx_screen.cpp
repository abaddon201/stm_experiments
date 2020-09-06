/*
 * zx_screen.cpp
 *
 *  Created on: Sep 6, 2020
 *      Author: user
 */

#include "zx_screen.h"

#include "ili9341.h"

ZxScreen::ZxScreen() {
    brightColors[0] = Tft9341::convert24to16(0x000000);
    brightColors[1] = Tft9341::convert24to16(0x0000FF);
    brightColors[2] = Tft9341::convert24to16(0xFF0000);
    brightColors[3] = Tft9341::convert24to16(0xFF00FF);
    brightColors[4] = Tft9341::convert24to16(0x00FF00);
    brightColors[5] = Tft9341::convert24to16(0x00FFFF);
    brightColors[6] = Tft9341::convert24to16(0xFFFF00);
    brightColors[7] = Tft9341::convert24to16(0xFFFFFF);

    darkColors[0] = Tft9341::convert24to16(0x000000);
    darkColors[1] = Tft9341::convert24to16(0x0000D7);
    darkColors[2] = Tft9341::convert24to16(0xD70000);
    darkColors[3] = Tft9341::convert24to16(0xD700D7);
    darkColors[4] = Tft9341::convert24to16(0x00D700);
    darkColors[5] = Tft9341::convert24to16(0x00D7D7);
    darkColors[6] = Tft9341::convert24to16(0xD7D700);
    darkColors[7] = Tft9341::convert24to16(0xD7D7D7);

}
