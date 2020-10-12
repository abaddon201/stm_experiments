/*
 * ui_element.h
 *
 *  Created on: 25 сент. 2020 г.
 *      Author: Abby
 */

#ifndef UI_ELEMENT_H_
#define UI_ELEMENT_H_

#include "ili9341.h"

class UIElement {
protected:
    int x, y;
    int width, height;
    Tft9341 *display;
public:
    UIElement(Tft9341 *display) :
            display { display } {
    }
    virtual void draw() = 0;
};

#endif /* UI_ELEMENT_H_ */
