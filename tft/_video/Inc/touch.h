/*
 * touch.h
 *
 *  Created on: Sep 7, 2020
 *      Author: Abby
 */

#ifndef INC_TOUCH_H_
#define INC_TOUCH_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include <stdio.h>

class Tft9341;

class TouchScreen {
    uint32_t xfac;
    uint32_t yfac;
    uint16_t xoff;
    uint16_t yoff;
    uint8_t touchtype;
    Tft9341 *display;

    void drawTouchPoint(uint16_t x, uint16_t y, uint16_t color);
    void drawBigPoint(uint16_t x, uint16_t y, uint16_t color);

    void adjustInfoShow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3,
            uint16_t y3, uint16_t fac);

    bool loadAdjustData();
    bool saveAdjustData();
    void makeAdjust();
public:
    bool init(Tft9341 *display);

    bool getTouch(uint16_t *x, uint16_t *y);
};

#endif /* INC_TOUCH_H_ */
