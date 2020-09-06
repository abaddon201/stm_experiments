#ifndef ILI9341_H_
#define ILI9341_H_

#include <stdlib.h>

#include "stm32f4xx_hal.h"
#include "fsmc.h"

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

extern uint16_t X_SIZE;
extern uint16_t Y_SIZE;

void TFT9341_ini(void);

uint32_t TFT9341_ReadReg(uint8_t r);

void TFT9341_SetRotation(unsigned char r);

void TFT9341_reset(void);
void TFT9341_FillScreen(uint16_t color);
void TFT9341_DrawPixel(int x, int y, uint16_t color);

#endif /* ILI9341_H_ */ 
