/*
 * ili9341.c
 *
 *  Created on: Sep 1, 2020
 *      Author: user
 */

#include "ili9341.h"
#include "ff.h"

#define  TFT9341_RESET_ACTIVE   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
#define  TFT9341_RESET_IDLE   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);

#define convert24to16(x) (((x & 0x00f80000) >> 8 ) | ((x & 0x0000fc00) >> 5) |((x & 0x000000f8) >> 3))

uint16_t X_SIZE = 0;
uint16_t Y_SIZE = 0;
uint32_t dtt = 0;

void TFT9341_ini(void) {
    TFT9341_reset();
    HAL_Delay(1000);
    dtt = TFT9341_ReadReg(0xD3);
}

void TFT9341_Delay(uint32_t dly) {
    uint32_t i;

    for (i = 0; i < dly; i++)
        ;
}

//————————————————————–

__STATIC_INLINE void DelayMicro(__IO uint32_t micros) {
    micros *= (SystemCoreClock / 1000000) / 5;

    while (micros--)
        ;
}

//————————————————————–

__STATIC_INLINE void TFT9341_SendCommand(unsigned char cmd) {
    TFT9341_ADDR_CMD = cmd;
}

//————————————————————–

__STATIC_INLINE void TFT9341_SendData(unsigned char dt) {
    TFT9341_ADDR_DATA = dt;
    //TFT9341_Delay(1);
}

//————————————————————–

uint32_t TFT9341_ReadReg(uint8_t r) {
    uint32_t id;
    uint8_t x;

    TFT9341_SendCommand(r);
    DelayMicro(50);

    x = TFT9341_ADDR_DATA;
    id = x;
    id <<= 8;
    DelayMicro(1);

    x = TFT9341_ADDR_DATA;
    id |= x;
    id <<= 8;
    DelayMicro(1);

    x = TFT9341_ADDR_DATA;
    id |= x;
    id <<= 8;
    DelayMicro(1);

    x = TFT9341_ADDR_DATA;
    id |= x;
    if (r == 0xEF) {
        id <<= 8;
        DelayMicro(5);

        x = TFT9341_ADDR_DATA;
        id |= x;
    }

    DelayMicro(150); //stabilization time

    return id;
}

//————————————————————–

void TFT9341_SetRotation(unsigned char r) {
    TFT9341_SendCommand(0x36);

    switch (r) {
    case 0:
        TFT9341_SendData(0x48);
        X_SIZE = 240;
        Y_SIZE = 320;
        break;
    case 1:
        TFT9341_SendData(0x28);
        X_SIZE = 320;
        Y_SIZE = 240;
        break;
    case 2:
        TFT9341_SendData(0x88);
        X_SIZE = 240;
        Y_SIZE = 320;
        break;
    case 3:
        TFT9341_SendData(0xE8);
        X_SIZE = 320;
        Y_SIZE = 240;
        break;
    }
}

void TFT9341_reset(void) {
    TFT9341_RESET_ACTIVE
    HAL_Delay(2);

    TFT9341_RESET_IDLE
    TFT9341_SendCommand(0x01); //Software Reset
    DelayMicro(1);

    TFT9341_SendCommand(0xCB); //Power Control A
    TFT9341_SendData(0x39);
    TFT9341_SendData(0x2C);
    TFT9341_SendData(0x00);
    TFT9341_SendData(0x34);
    TFT9341_SendData(0x02);
    DelayMicro(1);

    TFT9341_SendCommand(0xCF); //Power Control B
    TFT9341_SendData(0x00);
    TFT9341_SendData(0xC1);
    TFT9341_SendData(0x30);
    DelayMicro(1);

    TFT9341_SendCommand(0xE8); //Driver timing control A
    TFT9341_SendData(0x85);
    TFT9341_SendData(0x00);
    TFT9341_SendData(0x78);
    DelayMicro(1);

    TFT9341_SendCommand(0xEA); //Driver timing control B
    TFT9341_SendData(0x00);
    TFT9341_SendData(0x00);
    DelayMicro(1);

    TFT9341_SendCommand(0xED); //Power on Sequence control
    TFT9341_SendData(0x64);
    TFT9341_SendData(0x03);
    TFT9341_SendData(0x12);
    TFT9341_SendData(0x81);
    DelayMicro(1);

    TFT9341_SendCommand(0xF7); //Pump ratio control
    TFT9341_SendData(0x20);
    DelayMicro(1);

    TFT9341_SendCommand(0xC0); //Power Control 1
    TFT9341_SendData(0x10);
    DelayMicro(1);

    TFT9341_SendCommand(0xC1); //Power Control 2
    TFT9341_SendData(0x10);
    DelayMicro(1);

    TFT9341_SendCommand(0xC5); //VCOM Control 1
    TFT9341_SendData(0x3E);
    TFT9341_SendData(0x28);
    DelayMicro(1);

    TFT9341_SendCommand(0xC7); //VCOM Control 2
    TFT9341_SendData(0x86);
    DelayMicro(1);

    TFT9341_SetRotation(0);
    DelayMicro(1);

    TFT9341_SendCommand(0x3A); //Pixel Format Set
    TFT9341_SendData(0x55); //16bit
    DelayMicro(1);

    TFT9341_SendCommand(0xB1);
    TFT9341_SendData(0x00);
    TFT9341_SendData(0x18); // Частота кадров 79 Гц
    DelayMicro(1);

    TFT9341_SendCommand(0xB6); //Display Function Control
    TFT9341_SendData(0x08);
    TFT9341_SendData(0x82);
    TFT9341_SendData(0x27); //320 строк
    DelayMicro(1);

    TFT9341_SendCommand(0xF2); //Enable 3G (пока не знаю что это за режим)
    TFT9341_SendData(0x00); //не включаем
    DelayMicro(1);

    TFT9341_SendCommand(0x26); //Gamma set
    TFT9341_SendData(0x01); //Gamma Curve (G2.2) (Кривая цветовой гаммы)
    DelayMicro(1);

    TFT9341_SendCommand(0xE0); //Positive Gamma  Correction
    TFT9341_SendData(0x0F);
    TFT9341_SendData(0x31);
    TFT9341_SendData(0x2B);
    TFT9341_SendData(0x0C);
    TFT9341_SendData(0x0E);
    TFT9341_SendData(0x08);
    TFT9341_SendData(0x4E);
    TFT9341_SendData(0xF1);
    TFT9341_SendData(0x37);
    TFT9341_SendData(0x07);
    TFT9341_SendData(0x10);
    TFT9341_SendData(0x03);
    TFT9341_SendData(0x0E);
    TFT9341_SendData(0x09);
    TFT9341_SendData(0x00);
    DelayMicro(1);

    TFT9341_SendCommand(0xE1); //Negative Gamma  Correction
    TFT9341_SendData(0x00);
    TFT9341_SendData(0x0E);
    TFT9341_SendData(0x14);
    TFT9341_SendData(0x03);
    TFT9341_SendData(0x11);
    TFT9341_SendData(0x07);
    TFT9341_SendData(0x31);
    TFT9341_SendData(0xC1);
    TFT9341_SendData(0x48);
    TFT9341_SendData(0x08);
    TFT9341_SendData(0x0F);
    TFT9341_SendData(0x0C);
    TFT9341_SendData(0x31);
    TFT9341_SendData(0x36);
    TFT9341_SendData(0x0F);
    DelayMicro(1);

    TFT9341_SendCommand(0x11); //Выйдем из спящего режим
    HAL_Delay(150);

    TFT9341_SendCommand(0x29); //Включение дисплея
    TFT9341_SendData(0x2C);
    HAL_Delay(150);
    for (uint8_t i = 0; i < 3; i++) {
     TFT9341_SendData(0xFF);
     }
}

//————————————————————–

void TFT9341_Flood(uint16_t color, uint32_t len) {
    uint16_t blocks;
    uint8_t i, hi = color >> 8, lo = color;

    TFT9341_SendCommand(0x2C);
    /*TFT9341_SendData(hi);
    DelayMicro(1);

    TFT9341_SendData(lo);
    len--;*/
    blocks = (uint16_t) (len / 64); //64 pixels/block

    while (blocks--) {
        i = 16;
        do {
            TFT9341_SendData(hi);
            TFT9341_SendData(lo);
            TFT9341_SendData(hi);
            TFT9341_SendData(lo);
            TFT9341_SendData(hi);
            TFT9341_SendData(lo);
            TFT9341_SendData(hi);
            TFT9341_SendData(lo);
        } while (--i);
    }

    //Fill any remaining pixels(1 to 64)
/*
    for (i = (uint8_t) len & 63; i--;) {
        TFT9341_SendData(hi);
        TFT9341_SendData(lo);
    }*/
}

//————————————————————–

void TFT9341_SetAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    TFT9341_SendCommand(0x2A); //Column Addres Set
    TFT9341_SendData(x1 >> 8);
    TFT9341_SendData(x1 & 0xFF);
    TFT9341_SendData(x2 >> 8);
    TFT9341_SendData(x2 & 0xFF);
    DelayMicro(1);

    TFT9341_SendCommand(0x2B); //Page Addres Set
    TFT9341_SendData(y1 >> 8);
    TFT9341_SendData(y1 & 0xFF);
    TFT9341_SendData(y2 >> 8);
    TFT9341_SendData(y2 & 0xFF);
    DelayMicro(1);
}

//————————————————————–

void TFT9341_FillScreen(uint16_t color) {
    TFT9341_SetAddrWindow(0, 0, X_SIZE - 1, Y_SIZE - 1);
    TFT9341_Flood(color, (long) X_SIZE * (long) Y_SIZE);
}

void TFT9341_FillRectangle(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    TFT9341_SetAddrWindow(x1, y1, x2, y2);
    TFT9341_Flood(color, (uint16_t) (x2 - x1 + 1) * (uint16_t) (y2 - y1 + 1));
}

void TFT9341_DrawPixel(int x, int y, uint16_t color) {
    if ((x < 0) || (y < 0) || (x >= X_SIZE) || (y >= Y_SIZE)) {
        return;
    }
    TFT9341_SetAddrWindow(x, y, x, y);
    TFT9341_SendCommand(0x2C);
    TFT9341_SendData(color >> 8);
    TFT9341_SendData(color & 0xFF);
}

void TFT9341_DrawLine(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) {
        swap(x1, y1);
        swap(x2, y2);
    }

    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    int dx, dy;
    dx = x2 - x1;
    dy = abs(y2 - y1);

    int err = dx / 2;
    int ystep;

    if (y1 < y2) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x1 <= x2; x1++) {
        if (steep) {
            TFT9341_DrawPixel(y1, x1, color);
        } else {
            TFT9341_DrawPixel(x1, y1, color);
        }

        err -= dy;
        if (err < 0) {
            y1 += ystep;
            err += dx;
        }
    }
}

void TFT9341_DrawRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    TFT9341_DrawLine(color, x1, y1, x2, y1);
    TFT9341_DrawLine(color, x2, y1, x2, y2);
    TFT9341_DrawLine(color, x1, y1, x1, y2);
    TFT9341_DrawLine(color, x1, y2, x2, y2);
}

void TFT9341_DrawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color) {
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    TFT9341_DrawPixel(x0, y0 + r, color);
    TFT9341_DrawPixel(x0, y0 - r, color);
    TFT9341_DrawPixel(x0 + r, y0, color);
    TFT9341_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TFT9341_DrawPixel(x0 + x, y0 + y, color);
        TFT9341_DrawPixel(x0 - x, y0 + y, color);
        TFT9341_DrawPixel(x0 + x, y0 - y, color);
        TFT9341_DrawPixel(x0 - x, y0 - y, color);
        TFT9341_DrawPixel(x0 + y, y0 + x, color);
        TFT9341_DrawPixel(x0 - y, y0 + x, color);
        TFT9341_DrawPixel(x0 + y, y0 - x, color);
        TFT9341_DrawPixel(x0 - y, y0 - x, color);
    }
}

static FIL MyFile;
void TFT9341_DrawBitmap(uint16_t x, uint16_t y, char *s) {
    uint16_t i, h;
    uint32_t index = 0, width = 0, height = 0, bitpixel = 0;
    FRESULT res; /* FatFs function common result code */
    uint32_t bytesread;
    uint32_t tmpcolor;
    uint8_t *bufbmp = NULL;

    f_close(&MyFile);
    if (f_open(&MyFile, s, FA_READ) != FR_OK) {
        Error_Handler();
    } else {
        bufbmp = (uint8_t*) malloc(100);
        res = f_read(&MyFile, bufbmp, 90, (void*) &bytesread);
        if ((bytesread == 0) || (res != FR_OK)) {
            Error_Handler();
        } else {
            index = bufbmp[10]; //адрес начала данных в файле
            index |= bufbmp[11] << 8;
            index |= bufbmp[12] << 16;
            index |= bufbmp[13] << 24;
            width = bufbmp[18]; //ширина растра
            width |= bufbmp[19] << 8;
            width |= bufbmp[20] << 16;
            width |= bufbmp[21] << 24;
            height = bufbmp[22]; //высота растра
            height |= bufbmp[23] << 8;
            height |= bufbmp[24] << 16;
            height |= bufbmp[25] << 24;
            bitpixel = bufbmp[28]; //формат пикселя (бит/пиксель)
            bitpixel |= bufbmp[29] << 8;
            for (h = 0; h < height; h++) {
                for (i = 0; i < width; i++) {
                    switch (bitpixel) {
                    case 24:
                        f_lseek(&MyFile, index + ((height - h - 1) * width * 3) + (i * 3));
                        res = f_read(&MyFile, bufbmp, 3, (void*) &bytesread);
                        if ((bytesread == 0) || (res != FR_OK)) {
                            Error_Handler();
                        }
                        tmpcolor = bufbmp[0];
                        tmpcolor |= bufbmp[1] << 8;
                        tmpcolor |= bufbmp[2] << 16;
                        break;
                    }
                    if (((i + x) < width) | ((h + y) < height))
                        TFT9341_DrawPixel(i + x, h + y, (uint16_t) convert24to16(tmpcolor));
                }
            }
            f_close(&MyFile);
        }
        free(bufbmp);
    }
}
