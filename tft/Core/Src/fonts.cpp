/*
 * fonts.c
 *
 *  Created on: 2 сент. 2020 г.
 *      Author: user
 */

#include "fonts.h"

#include <string.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "ff.h"
#include "ili9341.h"

void FontSystem::loadFont(struct Font *font, uint16_t w, uint16_t h, const char *fname) {
    FIL *MyFile = new FIL();
    FRESULT res; /* FatFs function common result code */
    memset(MyFile, 0, sizeof(FIL));

    res = f_open(MyFile, fname, FA_READ);
    if (res != FR_OK) {
        Error_Handler();
        return;
    }
    font->Width = w;
    font->Height = h;

    font->bytesWidth = (w + 7) / 8;
    uint16_t symbolSize = h * font->bytesWidth;
    font->data = new uint8_t[95 * symbolSize];
    UINT bytesRead;
    res = f_read(MyFile, font->data, 95 * symbolSize, &bytesRead);
    if ((bytesRead == 0) || (res != FR_OK)) {
        delete[] font->data;
        font->data = 0;
        font->Width = 0;
        font->Height = 0;
        Error_Handler();
        return;
    }
    f_close(MyFile);
    delete MyFile;
}

void FontSystem::init() {
    loadFont(&Font8, 5, 8, "font8.bin");
    loadFont(&Font12, 7, 12, "font12.bin");
    loadFont(&Font16, 11, 16, "font16.bin");
    loadFont(&Font20, 14, 20, "font20.bin");
    loadFont(&Font24, 17, 24, "font24.bin");
}

void FontSystem::drawChar(struct Font *font, uint16_t x, uint16_t y, uint8_t s) {
    uint32_t i = 0, j = 0;
    uint16_t height, width;
    width = font->Width;
    height = font->Height;
    uint16_t fontsize = 0; //размер символа в байтах в таблице
    uint32_t line = 0; //горизонтальная линия пикселей шрифта
    uint8_t offset1; //разница между реальной шириной и шириной в байтах
    uint16_t offsetfile = 0; //Смещение символа в файле шрифта

    if ((x + font->Width) >= display->xSize())
        return;

    offset1 = font->bytesWidth * 8 - font->Width;
    fontsize = font->bytesWidth * font->Height;
    offsetfile = (s - ' ') * fontsize;

    //FIXME: maybe this must be in file read part
    //HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    //FIXME: end

    for (i = 0; i < height; i++) {
        if (font->bytesWidth == 1) {
            line = font->data[offsetfile + i];
        } else if (font->bytesWidth == 2) {
            line = (font->data[offsetfile + i * 2] << 8) | (font->data[offsetfile + i * 2 + 1]);
        } else {
            line = (font->data[offsetfile + i * 3] << 16) | (font->data[offsetfile + i * 3 + 1] << 8)
                    | (font->data[offsetfile + i * 3 + 2]);
        }
//                line=0xFFFF;

        uint16_t mask = 1 << (width + offset1 - 1);
        for (j = 0; j < width; j++) {
            if (line & mask) {
                display->drawPoint(x + j, y, BLACK);
            } else {
                display->drawPoint(x + j, y, WHITE);
            }
            mask >>= 1;
        }
        y++;
    }
}

/*void TFT9341_SetTextColor(uint16_t color) {
 lcdprop.TextColor = color;
 }

 //————————————————————–

 void TFT9341_SetBackColor(uint16_t color) {
 lcdprop.BackColor = color;
 }
 */

void FontSystem::drawString(FontSize fontSize, uint16_t x, uint16_t y, const char *str) {
    Font *font;
    switch (fontSize) {
    case FontSize::FONT8:
        font = &Font8;
        break;
    case FontSize::FONT12:
        font = &Font12;
        break;
    case FontSize::FONT16:
        font = &Font16;
        break;
    case FontSize::FONT20:
        font = &Font20;
        break;
    case FontSize::FONT24:
        font = &Font24;
        break;
    }

    while (*str) {
        drawChar(font, x, y, str[0]);
        x += font->Width;
        ++str;
    }
}

