/*
 * main_screen.h
 *
 *  Created on: 25 сент. 2020 г.
 *      Author: Abby
 */

#ifndef MAIN_SCREEN_H_
#define MAIN_SCREEN_H_

#include "ui_element.h"

class MainScreen {
    Tft9341 *display;
    UIElement *menuLeft[4];
    UIElement *menuRight[4];
public:
    MainScreen(Tft9341 *display) :
            display { display } {
    }
    void setLeftMenuButton(int idx, UIElement *element);
    void show();
    void hide();
};

#endif /* MAIN_SCREEN_H_ */
