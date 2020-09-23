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

//void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac);           //ПФК?Р???РЕП?

    bool getTouch(uint16_t *x, uint16_t *y);
};

#endif /* INC_TOUCH_H_ */
