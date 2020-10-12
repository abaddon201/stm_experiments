/*
 * button.h
 *
 *  Created on: 25 сент. 2020 г.
 *      Author: Abby
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "ui_element.h"

class Button: public UIElement {
    char *text;
    uint16_t color;
public:
    Button(Tft9341 *display, char *text, uint16_t color) :
            UIElement(display), text { text }, color { color } {

    }
    void draw() override;
};

#endif /* BUTTON_H_ */
