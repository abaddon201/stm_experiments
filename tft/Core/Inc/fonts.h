/*
 * fonts.h
 *
 *  Created on: Sep 2, 2020
 *      Author: Abby
 */

#ifndef INC_FONTS_H_
#define INC_FONTS_H_

#include <cstdint>

class Tft9341;

class FontSystem {
    Tft9341 *display;
public:
    enum class FontSize {
        FONT8, FONT12, FONT16, FONT20, FONT24
    };

    struct Font {
        uint16_t Width;
        uint16_t Height;
        uint16_t bytesWidth;
        uint8_t *data;
    };

    Font Font24;
    Font Font20;
    Font Font16;
    Font Font12;
    Font Font8;

    FontSystem(Tft9341 *display) :
            display { display } {
    }
    ;

    void init();
    void loadFont(struct Font *font, uint16_t w, uint16_t h, const char *fname);

    void drawChar(struct Font *font, uint16_t x, uint16_t y, uint8_t s);
    void drawString(FontSize fontSize, uint16_t x, uint16_t y, const char *str);
};

#endif /* INC_FONTS_H_ */
