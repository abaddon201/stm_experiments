/*
 * touch.h
 *
 *  Created on: Sep 7, 2020
 *      Author: user
 */

#ifndef INC_TOUCH_H_
#define INC_TOUCH_H_

//#include "sys.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define TP_PRES_DOWN 0x80
#define TP_CATH_PRES 0x40

#include <stdio.h>

typedef uint8_t u8;
typedef uint16_t u16;

typedef struct {
    u8 (*init)(void);
    u8 (*scan)(u8);
    void (*adjust)(void);
    u16 x0;
    u16 y0;
    u16 x;
    u16 y;
    u8 sta;

    float xfac;
    float yfac;
    short xoff;
    short yoff;

    u8 touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //Кдіц
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //КдИл

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //Кдіц
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //КдИл

#define PEN         PBin(11)     //T_PEN
#define DOUT        PCin(3)     //T_MISO
#define TDIN        PCout(2)   //T_MOSI
#define TCLK        PBout(10)    //T_SCK
#define TCS         PCout(13)   //T_CS

void TP_Write_Byte(u8 num);                     //Пт?ШЦЖР?Ж?Р?ИлТ??цКэ?Э
u16 TP_Read_AD(u8 CMD);                         //?БИ?AD?Є??Цµ
u16 TP_Read_XOY(u8 xy);                         //?шВЛ??µД?ш?к?БИ?(X/Y)
u8 TP_Read_XY(u16 *x, u16 *y);                   //Л???Пт?БИ?(X+Y)
u8 TP_Read_XY2(u16 *x, u16 *y);                  //?ш?УЗ?ВЛ??µДЛ???Пт?ш?к?БИ?
void TP_Drow_Touch_Point(u16 x, u16 y, u16 color);                  //??Т??ц?ш?кР???µг
void TP_Draw_Big_Point(u16 x, u16 y, u16 color);  //??Т??ц?уµг
u8 TP_Scan(u8 tp);                              //Й?Ги
void TP_Save_Adjdata(void);                     //???жР????ОКэ
u8 TP_Get_Adjdata(void);                        //?БИ?Р????ОКэ
void TP_Adjust(void);                           //??ГюЖБР???
u8 TP_Init(void);                               //?хК???

void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac);           //ПФК?Р???РЕП?

bool ILI9341_TouchGetCoordinates(uint16_t* x, uint16_t* y);

#endif /* INC_TOUCH_H_ */
