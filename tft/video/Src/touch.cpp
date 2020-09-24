/*
 * touch.cpp
 *
 *  Created on: Sep 7, 2020
 *      Author: Abby
 */

#include "touch.h"

#include <stdlib.h>
#include <math.h>

#include "config_file.h"
#include "delay.h"
#include "ili9341.h"

void TouchScreen::drawTouchPoint(uint16_t x, uint16_t y, uint16_t color) {
    display->drawLine(x - 12, y, x + 13, y, color);
    display->drawLine(x, y - 12, x, y + 13, color);
    display->drawPoint(x + 1, y + 1, color);
    display->drawPoint(x - 1, y + 1, color);
    display->drawPoint(x + 1, y - 1, color);
    display->drawPoint(x - 1, y - 1, color);
    display->drawCircle(x, y, 6, color);
}

void TouchScreen::drawBigPoint(uint16_t x, uint16_t y, uint16_t color) {
    display->drawPoint(x, y, color);
    display->drawPoint(x + 1, y, color);
    display->drawPoint(x, y + 1, color);
    display->drawPoint(x + 1, y + 1, color);

}

const char *TP_REMIND_MSG_TBL =
        "Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";

void TouchScreen::adjustInfoShow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
        uint16_t x3, uint16_t y3, uint16_t fac) {

    uint16_t color = WHITE;
    display->drawString(FontSystem::FontSize::FONT16, 40, 140, "x1:", color);
    display->drawString(FontSystem::FontSize::FONT16, 40 + 80, 140, "y1:", color);
    display->drawString(FontSystem::FontSize::FONT16, 40, 160, "x2:", color);
    display->drawString(FontSystem::FontSize::FONT16, 40 + 80, 160, "y2:", color);
    display->drawString(FontSystem::FontSize::FONT16, 40, 180, "x3:", color);
    display->drawString(FontSystem::FontSize::FONT16, 40 + 80, 180, "y3:", color);
    display->drawString(FontSystem::FontSize::FONT16, 40, 200, "x4:", color);
    display->drawString(FontSystem::FontSize::FONT16, 40 + 80, 200, "y4:", color);
    display->drawString(FontSystem::FontSize::FONT16, 40, 220, "fac is:", color);
    display->drawInt(FontSystem::FontSize::FONT16, 40 + 24, 140, x0, color);
    display->drawInt(FontSystem::FontSize::FONT16, 40 + 24 + 80, 140, y0, color);
    display->drawInt(FontSystem::FontSize::FONT16, 40 + 24, 160, x1, color);
    display->drawInt(FontSystem::FontSize::FONT16, 40 + 24 + 80, 160, y1, color);
    display->drawInt(FontSystem::FontSize::FONT16, 40 + 24, 180, x2, color);
    display->drawInt(FontSystem::FontSize::FONT16, 40 + 24 + 80, 180, y2, color);
    display->drawInt(FontSystem::FontSize::FONT16, 40 + 24, 200, x3, color);
    display->drawInt(FontSystem::FontSize::FONT16, 40 + 24 + 80, 200, y3, color);
    display->drawInt(FontSystem::FontSize::FONT16, 40 + 56, 220, fac, color);
}

void TouchScreen::makeAdjust() {
/*    uint16_t pos_temp[4][2];
    uint8_t cnt = 0;
    uint16_t d1, d2;
    uint32_t tem1, tem2;
    double fac;
    uint16_t outtime = 0;
    cnt = 0;
    display->fillScreen(WHITE);
    display->drawString(FontSystem::FontSize::FONT16, 10, 40, "Please use the stylus click", BLACK);
    display->drawString(FontSystem::FontSize::FONT16, 10, 56, "the cross on the screen.", BLACK);
    display->drawString(FontSystem::FontSize::FONT16, 10, 72, "The cross will always move", BLACK);
    display->drawString(FontSystem::FontSize::FONT16, 10, 88, "until the screen adjustment", BLACK);
    display->drawString(FontSystem::FontSize::FONT16, 10, 104, "is completed.", BLACK);

    drawTouchPoint(20, 20, RED);
    uint16_t sta = 0;
    xfac = 0;
    while (1) {
        tp_dev.scan(1);
        if ((sta & 0xc0) == TP_CATH_PRES) {
            outtime = 0;
            tp_dev.sta &= ~(1 << 6);

            pos_temp[cnt][0] = tp_dev.x;
            pos_temp[cnt][1] = tp_dev.y;
            cnt++;
            switch (cnt) {
            case 1:
                drawTouchPoint(20, 20, WHITE);
                drawTouchPoint(display->xSize() - 20, 20, RED);
                break;
            case 2:
                drawTouchPoint(display->xSize() - 20, 20, WHITE);
                drawTouchPoint(20, display->ySize() - 20, RED);
                break;
            case 3:
                drawTouchPoint(20, display->ySize() - 20, WHITE);
                drawTouchPoint(display->xSize() - 20, display->ySize() - 20, RED);
                break;
            case 4:
                tem1 = abs(pos_temp[0][0] - pos_temp[1][0]); //x1-x2
                tem2 = abs(pos_temp[0][1] - pos_temp[1][1]); //y1-y2
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = sqrt(tem1 + tem2);

                tem1 = abs(pos_temp[2][0] - pos_temp[3][0]); //x3-x4
                tem2 = abs(pos_temp[2][1] - pos_temp[3][1]); //y3-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = sqrt(tem1 + tem2);
                fac = (float) d1 / d2;
                if (fac < 0.95 || fac > 1.05 || d1 == 0 || d2 == 0) {
                    cnt = 0;
                    drawTouchPoint(display->xSize() - 20, display->ySize() - 20, WHITE);
                    drawTouchPoint(20, 20, RED);
                    adjustInfoShow(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
                            pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100);
                    continue;
                }
                tem1 = abs(pos_temp[0][0] - pos_temp[2][0]); //x1-x3
                tem2 = abs(pos_temp[0][1] - pos_temp[2][1]); //y1-y3
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = sqrt(tem1 + tem2);

                tem1 = abs(pos_temp[1][0] - pos_temp[3][0]); //x2-x4
                tem2 = abs(pos_temp[1][1] - pos_temp[3][1]); //y2-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = sqrt(tem1 + tem2);
                fac = (float) d1 / d2;
                if (fac < 0.95 || fac > 1.05) {
                    cnt = 0;
                    drawTouchPoint(display->xSize() - 20, display->ySize() - 20, WHITE);
                    drawTouchPoint(20, 20, RED);
                    adjustInfoShow(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
                            pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100);
                    continue;
                }

                tem1 = abs(pos_temp[1][0] - pos_temp[2][0]); //x1-x3
                tem2 = abs(pos_temp[1][1] - pos_temp[2][1]); //y1-y3
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = sqrt(tem1 + tem2);

                tem1 = abs(pos_temp[0][0] - pos_temp[3][0]); //x2-x4
                tem2 = abs(pos_temp[0][1] - pos_temp[3][1]); //y2-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = sqrt(tem1 + tem2);
                fac = (float) d1 / d2;
                if (fac < 0.95 || fac > 1.05) {
                    cnt = 0;
                    drawTouchPoint(display->xSize() - 20, display->ySize() - 20, WHITE);
                    drawTouchPoint(20, 20, RED);
                    adjustInfoShow(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0],
                            pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100);
                    continue;
                }
                xfac = (float) (display->xSize() - 40) / (pos_temp[1][0] - pos_temp[0][0]);
                xoff = (display->xSize() - xfac * (pos_temp[1][0] + pos_temp[0][0])) / 2;

                yfac = (float) (display->ySize() - 40) / (pos_temp[2][1] - pos_temp[0][1]);
                yoff = (display->ySize() - yfac * (pos_temp[2][1] + pos_temp[0][1])) / 2;
                if (abs(tp_dev.xfac) > 2 || abs(tp_dev.yfac) > 2) {
                    cnt = 0;
                    drawTouchPoint(display->xSize() - 20, display->ySize() - 20, WHITE);
                    drawTouchPoint(20, 20, RED);                             //»­µг1
                    display->drawString(FontSystem::FontSize::FONT16, 40, 26, "TP Need readjust!");
                    touchtype = !touchtype;
                    if (touchtype) {
                        CMD_RDX = 0X90;
                        CMD_RDY = 0XD0;
                    } else {
                        CMD_RDX = 0XD0;
                        CMD_RDY = 0X90;
                    }
                    continue;
                }
                display->fillScreen(WHITE);
                display->drawString(FontSystem::FontSize::FONT16, 35, 110, "Touch Screen Adjust OK!", BLUE);
                delay_ms(1000);
                saveAdjustData();
                display->fillScreen(WHITE);
                return;
            }
        }
        delay_ms(10);
        outtime++;
        if (outtime > 1000) {
            loadAdjustData();
            break;
        }
    }*/
}

bool TouchScreen::loadAdjustData() {
    xfac = config.get<uint32_t>(ConfigFile::TOUCH_PARAMS);
    yfac = config.get<uint32_t>(ConfigFile::TOUCH_PARAMS + 4);
    xoff = config.get<uint16_t>(ConfigFile::TOUCH_PARAMS + 8);
    yoff = config.get<uint16_t>(ConfigFile::TOUCH_PARAMS + 10);
    touchtype = config.get<uint8_t>(ConfigFile::TOUCH_PARAMS + 12);
    /*    if(tp_dev.touchtype)
     {
     CMD_RDX=0X90;
     CMD_RDY=0XD0;
     } else {
     CMD_RDX=0XD0;
     CMD_RDY=0X90;
     }*/
    return true;
}

bool TouchScreen::saveAdjustData() {
#define SAVE_ADDR_BASE 40
    config.put(ConfigFile::TOUCH_PARAMS, (uint32_t) xfac * 100000000);
    config.put(ConfigFile::TOUCH_PARAMS + 4, (uint32_t) yfac * 100000000);
    config.put(ConfigFile::TOUCH_PARAMS + 8, (uint16_t) xoff);
    config.put(ConfigFile::TOUCH_PARAMS + 10, (uint16_t) yoff);
    config.put(ConfigFile::TOUCH_PARAMS + 12, (uint8_t) touchtype);
    return config.save();
}

bool TouchScreen::init(Tft9341 *display) {
    uint16_t x;
    uint16_t y;
    this->display = display;
    getTouch(&x, &y);
    if (loadAdjustData()) {
        return false;
    } else {
        makeAdjust();
        saveAdjustData();
    }
    loadAdjustData();
    return true;
}

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)
#define PEN         PBin(11)     //T_PEN

#define TP_PRES_DOWN 0x80
#define TP_CATH_PRES 0x40

#define READ_X 0xD0
#define READ_Y 0x90
// change depending on screen orientation
#define ILI9341_TOUCH_SCALE_X 240
#define ILI9341_TOUCH_SCALE_Y 320
#define ILI9341_TOUCH_SPI_PORT hspi2
extern SPI_HandleTypeDef ILI9341_TOUCH_SPI_PORT;

#define ILI9341_TOUCH_MIN_RAW_X 1500
#define ILI9341_TOUCH_MAX_RAW_X 31000
#define ILI9341_TOUCH_MIN_RAW_Y 3276
#define ILI9341_TOUCH_MAX_RAW_Y 30110

bool TouchScreen::getTouch(uint16_t *x, uint16_t *y) {
    static const uint8_t cmd_read_x[] = { READ_X };
    static const uint8_t cmd_read_y[] = { READ_Y };
    static const uint8_t zeroes_tx[] = { 0x00, 0x00 };

    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
    uint8_t nsamples = 0;
    for (uint8_t i = 0; i < 16; i++) {
        volatile uint8_t p = PEN;
        if (p != 0) {
            break;
        }
        nsamples++;

        HAL_SPI_Transmit(&ILI9341_TOUCH_SPI_PORT, (uint8_t*) cmd_read_y, sizeof(cmd_read_y), HAL_MAX_DELAY);
        uint8_t y_raw[2];
        HAL_SPI_TransmitReceive(&ILI9341_TOUCH_SPI_PORT, (uint8_t*) zeroes_tx, y_raw, sizeof(y_raw), HAL_MAX_DELAY);

        HAL_SPI_Transmit(&ILI9341_TOUCH_SPI_PORT, (uint8_t*) cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
        uint8_t x_raw[2];
        HAL_SPI_TransmitReceive(&ILI9341_TOUCH_SPI_PORT, (uint8_t*) zeroes_tx, x_raw, sizeof(x_raw), HAL_MAX_DELAY);

        avg_x += (((uint16_t) x_raw[0]) << 8) | ((uint16_t) x_raw[1]);
        avg_y += (((uint16_t) y_raw[0]) << 8) | ((uint16_t) y_raw[1]);
    }

    if (nsamples < 16)
        return false;

    uint32_t raw_x = (avg_x / 16);
    if (raw_x < ILI9341_TOUCH_MIN_RAW_X)
        raw_x = ILI9341_TOUCH_MIN_RAW_X;
    if (raw_x > ILI9341_TOUCH_MAX_RAW_X)
        raw_x = ILI9341_TOUCH_MAX_RAW_X;

    uint32_t raw_y = (avg_y / 16);
    if (raw_y < ILI9341_TOUCH_MIN_RAW_X)
        raw_y = ILI9341_TOUCH_MIN_RAW_Y;
    if (raw_y > ILI9341_TOUCH_MAX_RAW_Y)
        raw_y = ILI9341_TOUCH_MAX_RAW_Y;

    // Uncomment this line to calibrate touchscreen:
    // UART_Printf("raw_x = %d, raw_y = %d\r\n", x, y);

    // FIXME: apply adjustments
    *x = (raw_x - ILI9341_TOUCH_MIN_RAW_X) * ILI9341_TOUCH_SCALE_X
            / (ILI9341_TOUCH_MAX_RAW_X - ILI9341_TOUCH_MIN_RAW_X);
    *y = (raw_y - ILI9341_TOUCH_MIN_RAW_Y) * ILI9341_TOUCH_SCALE_Y
            / (ILI9341_TOUCH_MAX_RAW_Y - ILI9341_TOUCH_MIN_RAW_Y);

    return true;
}
