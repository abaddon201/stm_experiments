/*
 * task_display.cpp
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: Abby
 */

#include "task_display.h"

#include "fonts.h"
#include "ili9341.h"
#include "performance.h"
#include "touch.h"

#include <string.h>

Tft9341 *display;
TouchScreen *touchScreen;

void displayTask(void *params) {
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
