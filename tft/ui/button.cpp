/*
 * button.cpp
 *
 *  Created on: 25 сент. 2020 г.
 *      Author: Abby
 */

#include "button.h"

void Button::draw() {
    display->drawRect(x, y, x + width, y + width, color);
    display->drawString(FontSystem::FontSize::FONT16, x + 2, y + 2, text);
}
