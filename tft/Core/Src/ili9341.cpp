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

void Tft9341::init() {
//    uint32_t dtt = 0;

    reset();
    HAL_Delay(1000);
    //read display Id
//    dtt = TFT9341_ReadReg(0xD3);
}

void Tft9341::delay(uint32_t dly) {
    uint32_t i;

    for (i = 0; i < dly; i++)
        ;
}

void Tft9341::delayMicro(__IO uint32_t micros) {
    micros *= (SystemCoreClock / 1000000) / 5;

    while (micros--)
        ;
}

uint32_t Tft9341::readReg(uint8_t r) {
    uint32_t id;
    uint8_t x;

    sendCommand(r);
    delayMicro(50);

    x = TFT9341_ADDR_DATA;
    id = x;
    id <<= 8;
    delayMicro(1);

    x = TFT9341_ADDR_DATA;
    id |= x;
    id <<= 8;
    delayMicro(1);

    x = TFT9341_ADDR_DATA;
    id |= x;
    id <<= 8;
    delayMicro(1);

    x = TFT9341_ADDR_DATA;
    id |= x;
    if (r == 0xEF) {
        id <<= 8;
        delayMicro(5);

        x = TFT9341_ADDR_DATA;
        id |= x;
    }

    delayMicro(150); //stabilization time

    return id;
}

void Tft9341::setRotation(unsigned char r) {
    sendCommand(0x36);

    switch (r) {
    case 0:
        sendData(0x48);
        X_SIZE = 240;
        Y_SIZE = 320;
        break;
    case 1:
        sendData(0x28);
        X_SIZE = 320;
        Y_SIZE = 240;
        break;
    case 2:
        sendData(0x88);
        X_SIZE = 240;
        Y_SIZE = 320;
        break;
    case 3:
        sendData(0xE8);
        X_SIZE = 320;
        Y_SIZE = 240;
        break;
    }
}

void Tft9341::reset(void) {
    TFT9341_RESET_ACTIVE
    HAL_Delay(2);

    TFT9341_RESET_IDLE
    sendCommand(0x01); //Software Reset
    delayMicro(1);

    sendCommand(0xCB); //Power Control A
    sendData(0x39);
    sendData(0x2C);
    sendData(0x00);
    sendData(0x34);
    sendData(0x02);
    delayMicro(1);

    sendCommand(0xCF); //Power Control B
    sendData(0x00);
    sendData(0xC1);
    sendData(0x30);
    delayMicro(1);

    sendCommand(0xE8); //Driver timing control A
    sendData(0x85);
    sendData(0x00);
    sendData(0x78);
    delayMicro(1);

    sendCommand(0xEA); //Driver timing control B
    sendData(0x00);
    sendData(0x00);
    delayMicro(1);

    sendCommand(0xED); //Power on Sequence control
    sendData(0x64);
    sendData(0x03);
    sendData(0x12);
    sendData(0x81);
    delayMicro(1);

    sendCommand(0xF7); //Pump ratio control
    sendData(0x20);
    delayMicro(1);

    sendCommand(0xC0); //Power Control 1
    sendData(0x10);
    delayMicro(1);

    sendCommand(0xC1); //Power Control 2
    sendData(0x10);
    delayMicro(1);

    sendCommand(0xC5); //VCOM Control 1
    sendData(0x3E);
    sendData(0x28);
    delayMicro(1);

    sendCommand(0xC7); //VCOM Control 2
    sendData(0x86);
    delayMicro(1);

    setRotation(0);
    delayMicro(1);

    sendCommand(0x3A); //Pixel Format Set
    sendData(0x55); //16bit
    delayMicro(1);

    sendCommand(0xB1);
    sendData(0x00);
    sendData(0x18); // Частота кадров 79 Гц
    delayMicro(1);

    sendCommand(0xB6); //Display Function Control
    sendData(0x08);
    sendData(0x82);
    sendData(0x27); //320 строк
    delayMicro(1);

    sendCommand(0xF2); //Enable 3G (пока не знаю что это за режим)
    sendData(0x00); //не включаем
    delayMicro(1);

    sendCommand(0x26); //Gamma set
    sendData(0x01); //Gamma Curve (G2.2) (Кривая цветовой гаммы)
    delayMicro(1);

    sendCommand(0xE0); //Positive Gamma  Correction
    sendData(0x0F);
    sendData(0x31);
    sendData(0x2B);
    sendData(0x0C);
    sendData(0x0E);
    sendData(0x08);
    sendData(0x4E);
    sendData(0xF1);
    sendData(0x37);
    sendData(0x07);
    sendData(0x10);
    sendData(0x03);
    sendData(0x0E);
    sendData(0x09);
    sendData(0x00);
    delayMicro(1);

    sendCommand(0xE1); //Negative Gamma  Correction
    sendData(0x00);
    sendData(0x0E);
    sendData(0x14);
    sendData(0x03);
    sendData(0x11);
    sendData(0x07);
    sendData(0x31);
    sendData(0xC1);
    sendData(0x48);
    sendData(0x08);
    sendData(0x0F);
    sendData(0x0C);
    sendData(0x31);
    sendData(0x36);
    sendData(0x0F);
    delayMicro(1);

    sendCommand(0x11); //Выйдем из спящего режим
    HAL_Delay(150);

    sendCommand(0x29); //Включение дисплея
    sendData(0x2C);
    HAL_Delay(150);
    for (uint8_t i = 0; i < 3; i++) {
        sendData(0xFF);
    }
}

//————————————————————–

void Tft9341::flood(uint16_t color, uint32_t len) {
    uint16_t blocks;
    uint8_t i, hi = color >> 8, lo = color;

    sendCommand(0x2C);
    /*TFT9341_SendData(hi);
     DelayMicro(1);

     TFT9341_SendData(lo);
     len--;*/
    blocks = (uint16_t) (len / 64); //64 pixels/block

    while (blocks--) {
        i = 16;
        do {
            sendData(hi);
            sendData(lo);
            sendData(hi);
            sendData(lo);
            sendData(hi);
            sendData(lo);
            sendData(hi);
            sendData(lo);
        } while (--i);
    }

    //Fill any remaining pixels(1 to 64)
    /*
     for (i = (uint8_t) len & 63; i--;) {
     TFT9341_SendData(hi);
     TFT9341_SendData(lo);
     }*/
}

void Tft9341::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2,
        uint16_t y2) {
    sendCommand(0x2A); //Column Addres Set
    sendData(x1 >> 8);
    sendData(x1 & 0xFF);
    sendData(x2 >> 8);
    sendData(x2 & 0xFF);
    //DelayMicro(1);

    sendCommand(0x2B); //Page Addres Set
    sendData(y1 >> 8);
    sendData(y1 & 0xFF);
    sendData(y2 >> 8);
    sendData(y2 & 0xFF);
    //DelayMicro(1);
}

void Tft9341::fillScreen(uint16_t color) {
    setAddrWindow(0, 0, X_SIZE - 1, Y_SIZE - 1);
    flood(color, (long) X_SIZE * (long) Y_SIZE);
}

void Tft9341::fillRectangle(uint16_t color, uint16_t x1, uint16_t y1,
        uint16_t x2, uint16_t y2) {
    setAddrWindow(x1, y1, x2, y2);
    flood(color, (uint16_t) (x2 - x1 + 1) * (uint16_t) (y2 - y1 + 1));
}

void Tft9341::drawPixel(int x, int y, uint16_t color) {
    if ((x < 0) || (y < 0) || (x >= X_SIZE) || (y >= Y_SIZE)) {
        return;
    }
    setAddrWindow(x, y, x, y);
    sendCommand(0x2C);
    // DelayMicro(1);
    sendData(color >> 8);
    sendData(color & 0xFF);
}

void Tft9341::drawLine(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2,
        uint16_t y2) {
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
            drawPixel(y1, x1, color);
        } else {
            drawPixel(x1, y1, color);
        }

        err -= dy;
        if (err < 0) {
            y1 += ystep;
            err += dx;
        }
    }
}

void Tft9341::drawRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2,
        uint16_t y2) {
    drawLine(color, x1, y1, x2, y1);
    drawLine(color, x2, y1, x2, y2);
    drawLine(color, x1, y1, x1, y2);
    drawLine(color, x1, y2, x2, y2);
}

void Tft9341::drawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color) {
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

//FIXME: rewrite it to use preloaded images
/*
 static FIL MyFile;
 void TFT9341_DrawBitmap(uint16_t x, uint16_t y, char *s) {
 uint16_t i, h;
 uint32_t index = 0, width = 0, height = 0, bitpixel = 0;
 FRESULT res; // FatFs function common result code
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
 f_lseek(&MyFile,
 index + ((height - h - 1) * width * 3)
 + (i * 3));
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
 TFT9341_DrawPixel(i + x, h + y,
 (uint16_t) convert24to16(tmpcolor));
 }
 }
 f_close(&MyFile);
 }
 free(bufbmp);
 }
 }
 */
