/*
 * task_display.cpp
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: user
 */

#include "task_display.h"

#include "fonts.h"
#include "ili9341.h"
#include "performance.h"
#include "touch.h"

#include "fatfs.h"

#include <string.h>

Tft9341 *display;
TouchScreen *touchScreen;

void displayTask(void *params) {
    char SD_Path[4]; /* SD logical drive path */

    FRESULT res; /* FatFs function common result code */
    memset(&SDFatFS, 0, sizeof(FATFS));
    res = f_mount(&SDFatFS, (TCHAR const*) SD_Path, 1);
    if (res != FR_OK) {
        Error_Handler();
    }
    display = new Tft9341();
    touchScreen = new TouchScreen();

    display->init();
    touchScreen->init(display);
    uint16_t x, y;
    bool pressed;
    while (1) {
        Performance p { };
        display->fillScreen(BLUE);
        display->drawString(FontSystem::FontSize::FONT16, 0, 62, "Hello world");
        pressed = touchScreen->getTouch(&x, &y);
    }
}

