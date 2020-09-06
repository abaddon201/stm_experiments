/*
 * fonts.h
 *
 *  Created on: Sep 2, 2020
 *      Author: user
 */

#ifndef INC_FONTS_H_
#define INC_FONTS_H_

#include <cstdint>

class FontSystem {
public:
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

    void init();
    void loadFont(struct Font *font, uint16_t w, uint16_t h, const char *fname);

    void drawChar(struct Font *font, uint16_t x, uint16_t y, uint8_t s);
    void drawString(Font* font, uint16_t x, uint16_t y, char *str);
};


#endif /* INC_FONTS_H_ */
