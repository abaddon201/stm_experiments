#ifndef ILI9341_H_
#define ILI9341_H_

#include <stdlib.h>
#include <stm32f4xx_hal.h>
#include "fsmc.h"

#include "../../_video/Inc/fonts.h"

#define TFT9341_ADDR_CMD        *(uint8_t*)0x60000000
#define TFT9341_ADDR_DATA        *(uint8_t*)0x60010000

#define swap(a,b) {int16_t t=a;a=b;b=t;}

#define  BLACK 0x0000
#define  BLUE 0x001F
#define  RED 0x0F800
#define  GREEN 0x07E0
#define  CYAN 0x07FF
#define  MAGENTA 0xF81F
#define  YELLOW 0xFFE0
#define  WHITE 0xFFFF

/**
 * class for working with ili9341 display controller
 */
class Tft9341 {
    uint16_t X_SIZE;
    uint16_t Y_SIZE;
    FontSystem *fonts;

    uint32_t readReg(uint8_t r);

    inline void sendCommand(unsigned char cmd) {
        TFT9341_ADDR_CMD = cmd;
    }

    inline void sendData(unsigned char dt) {
        TFT9341_ADDR_DATA = dt;
    }

    void flood(uint16_t color, uint32_t len);

    void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

public:
    static uint16_t convert24to16(uint32_t x) {
        return (((x & 0x00f80000) >> 8) | ((x & 0x0000fc00) >> 5) | ((x & 0x000000f8) >> 3));
    }

    // init/reset
    void init();
    void reset();

    //display control
    void setRotation(unsigned char r);

    uint16_t xSize() {
        return X_SIZE;
    }

    uint16_t ySize() {
        return Y_SIZE;
    }

    // primitive drawings
    void fillScreen(uint16_t color);
    void fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void drawPoint(int x, int y, uint16_t color);
    void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void drawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color);
    void drawString(FontSystem::FontSize fontSize, uint16_t x, uint16_t y, const char *str);
    void drawInt(FontSystem::FontSize fontSize, uint16_t x, uint16_t y, int value);

    void drawString(FontSystem::FontSize fontSize, uint16_t x, uint16_t y, const char *str, uint16_t color);
    void drawInt(FontSystem::FontSize fontSize, uint16_t x, uint16_t y, int value, uint16_t color);
};

#endif /* ILI9341_H_ */ 
