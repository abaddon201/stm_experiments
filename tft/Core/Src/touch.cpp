/*
 * touch.cpp
 *
 *  Created on: Sep 7, 2020
 *      Author: user
 */




//////////////////////////////////////////////////////////////////////////////////
//???МРтЦ???С?П?К?УГ??О?????ХЯРн?Й????µГУГУЪЖдЛьИОєОУГН?
//?вКФУ??ю?єµ?Ж??ъSTM32F407ZGT6,ХэµгФ??УExplorer STM32F4?Є???е,Ц?Жµ168MHZ????Хс12MHZ
//QDtech-TFTТє??Зэ?? for STM32 FSMC
//xiao?л@ShenZhen QDtech co.,LTD
//??Л?НшХ?:www.qdtft.com
//МФ??НшХ??єhttp://qdtech.taobao.com
//wiki??КхНшХ??єhttp://www.lcdwiki.com
//ОТЛ?Мб????КхЦ??Ц??ИОєО??КхОКМв??У?ЛжК???Б?С?П?
//?М??(??Хж) :+86 0755-23594567
//КЦ?ъ:15989313508???л????
//УКПд:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//??КхЦ??ЦQQ:3002773612  3002778157
//??Кх??Б?QQИє:324828016
//????ИХЖЪ:2018/08/09
//?ж???єV1.0
//?жИ?ЛщУР??µБ?ж?Ш????
//Copyright(C) ЙоЫЪКРИ???µз?У??КхУРПЮ??Л? 2018-2028
//All rights reserved
/****************************************************************************************************
//?ЛД??й?ЙТФЦ??У?еИлХэµгФ??УExplorer STM32F4?Є???еTFTLCD?е?Ы??ОЮРиКЦ???УПЯ
//STM32Б??УТэ?ЕКЗЦ?TFTLCD?е?ЫТэ?ЕДЪ??Б??УµДSTM32Тэ?Е
//=========================================µзФ??УПЯ================================================//
//     LCDД??й             TFTLCD?е?ЫТэ?Е        STM32Б??УТэ?Е
//      VDD       --->         5V/3.3              DC5V/3.3V          //µзФ?
//      GND       --->          GND                  GND              //µзФ?µШ
//=======================================Тє??ЖБКэ?ЭПЯ?УПЯ==========================================//
//??Д??йД?ИПКэ?Э?ЬПЯАаРНОЄ16О????Ъ?ЬПЯ
//     LCDД??й             TFTLCD?е?ЫТэ?Е        STM32Б??УТэ?Е
//      DB0       --->          D0                   PD14        -|
//      DB1       --->          D1                   PD15         |
//      DB2       --->          D2                   PD0          |
//      DB3       --->          D3                   PD1          |
//      DB4       --->          D4                   PE7          |
//      DB5       --->          D5                   PE8          |
//      DB6       --->          D6                   PE9          |
//      DB7       --->          D7                   PE10         |
//Из?ыК?УГ8О?Д?К???ЗлК?УГПВГж?Я8О????ЪКэ?ЭТэ?Е                    |===>Тє??ЖБ16О????ЪКэ?ЭРЕєЕ
//      DB8       --->          D8                   PE11         |
//      DB9       --->          D9                   PE12         |
//      DB10      --->          D10                  PE13         |
//      DB11      --->          D11                  PE14         |
//      DB12      --->          D12                  PE15         |
//      DB13      --->          D13                  PD8          |
//      DB14      --->          D14                  PD9          |
//      DB15      --->          D15                  PD10        -|
//=======================================Тє??ЖБ?ШЦЖПЯ?УПЯ==========================================//
//     LCDД??й                   TFTLCD?е?ЫТэ?Е        STM32Б??УТэ?Е
//       WR       --->          WR                   PD5             //Тє??ЖБР?Кэ?Э?ШЦЖРЕєЕ
//       RD       --->          RD                   PD4             //Тє??ЖБ?БКэ?Э?ШЦЖРЕєЕ
//       RS       --->          RS                   PF12            //Тє??ЖБКэ?Э/ГьБо?ШЦЖРЕєЕ
//       RST      --->          RST                ??О?Тэ?Е          //Тє??ЖБ??О??ШЦЖРЕєЕ
//       CS       --->          CS                   PG12            //Тє??ЖБЖ?С??ШЦЖРЕєЕ
//       BL       --->          BL                   PB15            //Тє??ЖБ???в?ШЦЖРЕєЕ
//=========================================??ГюЖБ???УПЯ=========================================//
//Из?ыД??й???ш??Гю??ДЬ?тХЯ?шУР??Гю??ДЬ??µ?КЗ??РиТЄ??Гю??ДЬ??Фт??РиТЄ?шРР??ГюЖБ?УПЯ
//     LCDД??й             TFTLCD?е?ЫТэ?Е        STM32Б??УТэ?Е
//      PEN       --->          PEN                  PB1             //??ГюЖБ??ГюЦР?ПРЕєЕ
//      MISO      --->          MISO                 PB2             //??ГюЖБSPI?ЬПЯ?БРЕєЕ
//      MOSI      --->          MOSI                 PF11            //??ГюЖБSPI?ЬПЯР?РЕєЕ
//      T_CS      --->          TCS                  PC13            //??ГюЖБЖ?С??ШЦЖРЕєЕ
//      CLK       --->          CLK                  PB0             //ґҐГюЖБSPIЧЬПЯК±ЦУРЕєЕ
**************************************************************************************************/
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/
#include "touch.h"
//#include "lcd.h"
//#include "delay.h"
#include "stdlib.h"
#include "math.h"
//#include "24cxx.h"
//#include "gui.h"



void delay_us(uint32_t dly) {
    uint32_t i;

    for (i = 0; i < dly; i++)
        ;
}

void delay_ms(__IO uint32_t micros) {
    micros *= (SystemCoreClock / 1000000) / 5;

    while (micros--)
        ;
}



_m_tp_dev tp_dev=
{
    TP_Init,
    TP_Scan,
    TP_Adjust,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
//Д¬ИПОЄtouchtype=0µДКэѕЭ.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

/*****************************************************************************
 * @name       :void TP_Write_Byte(u8 num)
 * @date       :2018-08-09
 * @function   :Write a byte data to the touch screen IC with SPI bus
 * @parameters :num:Data to be written
 * @retvalue   :None
******************************************************************************/
void TP_Write_Byte(u8 num)
{
    u8 count=0;
    for(count=0;count<8;count++)
    {
        if(num&0x80)TDIN=1;
        else TDIN=0;
        num<<=1;
        TCLK=0;
        delay_us(1);
        TCLK=1;     //ЙПЙэСШУРР§
    }
}

/*****************************************************************************
 * @name       :u16 TP_Read_AD(u8 CMD)
 * @date       :2018-08-09
 * @function   :Reading adc values from touch screen IC with SPI bus
 * @parameters :CMD:Read command,0xD0 for x,0x90 for y
 * @retvalue   :Read data
******************************************************************************/
u16 TP_Read_AD(u8 CMD)
{
    u8 count=0;
    u16 Num=0;
    TCLK=0;     //ПИА­µНК±ЦУ
    TDIN=0;     //А­µНКэѕЭПЯ
    TCS=0;      //СЎЦРґҐГюЖБIC
    TP_Write_Byte(CMD);//·ўЛНГьБоЧЦ
    delay_us(6);//ADS7846µДЧЄ»»К±јдЧоі¤ОЄ6us
    TCLK=0;
    delay_us(1);
    TCLK=1;     //ёш1ёцК±ЦУЈ¬ЗеіэBUSY
    delay_us(1);
    TCLK=0;
    for(count=0;count<16;count++)//¶Біц16О»КэѕЭ,Ц»УРёЯ12О»УРР§
    {
        Num<<=1;
        TCLK=0; //ПВЅµСШУРР§
        delay_us(1);
        TCLK=1;
        if(DOUT)Num++;
    }
    Num>>=4;    //Ц»УРёЯ12О»УРР§.
    TCS=1;      //КН·ЕЖ¬СЎ
    return(Num);
//#endif
}

#define READ_TIMES 5    //¶БИЎґОКэ
#define LOST_VAL 1      //¶ЄЖъЦµ
/*****************************************************************************
 * @name       :u16 TP_Read_XOY(u8 xy)
 * @date       :2018-08-09
 * @function   :Read the touch screen coordinates (x or y),
                                Read the READ_TIMES secondary data in succession
                                and sort the data in ascending order,
                                Then remove the lowest and highest number of LOST_VAL
                                and take the average
 * @parameters :xy:Read command(CMD_RDX/CMD_RDY)
 * @retvalue   :Read data
******************************************************************************/
u16 TP_Read_XOY(u8 xy)
{
    u16 i, j;
    u16 buf[READ_TIMES];
    u16 sum=0;
    u16 temp;
    for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);
    for(i=0;i<READ_TIMES-1; i++)//ЕЕРт
    {
        for(j=i+1;j<READ_TIMES;j++)
        {
            if(buf[i]>buf[j])//ЙэРтЕЕБР
            {
                temp=buf[i];
                buf[i]=buf[j];
                buf[j]=temp;
            }
        }
    }
    sum=0;
    for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
    temp=sum/(READ_TIMES-2*LOST_VAL);
    return temp;
}

/*****************************************************************************
 * @name       :u8 TP_Read_XY(u16 *x,u16 *y)
 * @date       :2018-08-09
 * @function   :Read touch screen x and y coordinates,
                                The minimum value can not be less than 100
 * @parameters :x:Read x coordinate of the touch screen
                                y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/
u8 TP_Read_XY(u16 *x,u16 *y)
{
    u16 xtemp,ytemp;
    xtemp=TP_Read_XOY(CMD_RDX);
    ytemp=TP_Read_XOY(CMD_RDY);
    //if(xtemp<100||ytemp<100)return 0;//¶БКэК§°Ь
    *x=xtemp;
    *y=ytemp;
    return PEN;//¶БКэіЙ№¦
}

#define ERR_RANGE 50 //ОуІо·¶О§
/*****************************************************************************
 * @name       :u8 TP_Read_XY2(u16 *x,u16 *y)
 * @date       :2018-08-09
 * @function   :Read the touch screen coordinates twice in a row,
                                and the deviation of these two times can not exceed ERR_RANGE,
                                satisfy the condition, then think the reading is correct,
                                otherwise the reading is wrong.
                                This function can greatly improve the accuracy.
 * @parameters :x:Read x coordinate of the touch screen
                                y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/
u8 TP_Read_XY2(u16 *x,u16 *y)
{
    u16 x1,y1;
    u16 x2,y2;
    u8 flag;
    flag=TP_Read_XY(&x1,&y1);
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);
    if(flag==0)return(0);
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//З°єуБЅґОІЙСщФЪ+-50ДЪ
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;
}

/*****************************************************************************
 * @name       :void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
 * @date       :2018-08-09
 * @function   :Draw a touch point,Used to calibrate
 * @parameters :x:Read x coordinate of the touch screen
                                y:Read y coordinate of the touch screen
                                color:the color value of the touch point
 * @retvalue   :None
******************************************************************************/
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
    /*POINT_COLOR=color;
    LCD_DrawLine(x-12,y,x+13,y);//єбПЯ
    LCD_DrawLine(x,y-12,x,y+13);//КъПЯ
    LCD_DrawPoint(x+1,y+1);
    LCD_DrawPoint(x-1,y+1);
    LCD_DrawPoint(x+1,y-1);
    LCD_DrawPoint(x-1,y-1);
    gui_circle(x,y,POINT_COLOR,6,0);//»­ЦРРДИ¦*/
}

/*****************************************************************************
 * @name       :void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
 * @date       :2018-08-09
 * @function   :Draw a big point(2*2)
 * @parameters :x:Read x coordinate of the point
                                y:Read y coordinate of the point
                                color:the color value of the point
 * @retvalue   :None
******************************************************************************/
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{
    /*POINT_COLOR=color;
    LCD_DrawPoint(x,y);//ЦРРДµг
    LCD_DrawPoint(x+1,y);
    LCD_DrawPoint(x,y+1);
    LCD_DrawPoint(x+1,y+1);*/
}

/*****************************************************************************
 * @name       :u8 TP_Scan(u8 tp)
 * @date       :2018-08-09
 * @function   :Scanning touch event
 * @parameters :tp:0-screen coordinate
                                     1-Physical coordinates(For special occasions such as calibration)
 * @retvalue   :Current touch screen status,
                                0-no touch
                                1-touch
******************************************************************************/
u8 TP_Scan(u8 tp)
{
    if(PEN==0)//УР°ґјь°ґПВ
    {
        if(tp)TP_Read_XY2(&tp_dev.x,&tp_dev.y);//¶БИЎОпАнЧш±к
        else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))//¶БИЎЖБД»Чш±к
        {
            tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;//Ѕ«Ѕб№ыЧЄ»»ОЄЖБД»Чш±к
            tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;
        }
        if((tp_dev.sta&TP_PRES_DOWN)==0)//Ц®З°Г»УР±»°ґПВ
        {
            tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//°ґјь°ґПВ
            tp_dev.x0=tp_dev.x;//јЗВјµЪТ»ґО°ґПВК±µДЧш±к
            tp_dev.y0=tp_dev.y;
        }
    }else
    {
        if(tp_dev.sta&TP_PRES_DOWN)//Ц®З°КЗ±»°ґПВµД
        {
            tp_dev.sta&=~(1<<7);//±кјЗ°ґјьЛЙїЄ
        }else//Ц®З°ѕНГ»УР±»°ґПВ
        {
            tp_dev.x0=0;
            tp_dev.y0=0;
            tp_dev.x=0xffff;
            tp_dev.y=0xffff;
        }
    }
    return tp_dev.sta&TP_PRES_DOWN;//·µ»Шµ±З°µДґҐЖБЧґМ¬
}

//////////////////////////////////////////////////////////////////////////
//±ЈґжФЪEEPROMАпГжµДµШЦ·Зшјд»щЦ·,ХјУГ13ёцЧЦЅЪ(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
/*****************************************************************************
 * @name       :void TP_Save_Adjdata(void)
 * @date       :2018-08-09
 * @function   :Save calibration parameters
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TP_Save_Adjdata(void)
{
    /*s32 temp;
    //±ЈґжРЈХэЅб№ы!
    temp=tp_dev.xfac*100000000;//±ЈґжxРЈХэТтЛШ
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);
    temp=tp_dev.yfac*100000000;//±ЈґжyРЈХэТтЛШ
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
    //±ЈґжxЖ«ТЖБї
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);
    //±ЈґжyЖ«ТЖБї
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);
    //±ЈґжґҐЖБАаРН
    AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);
    temp=0X0A;//±кјЗРЈЧј№эБЛ
    AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp);*/
}

/*****************************************************************************
 * @name       :u8 TP_Get_Adjdata(void)
 * @date       :2018-08-09
 * @function   :Gets the calibration values stored in the EEPROM
 * @parameters :None
 * @retvalue   :1-get the calibration values successfully
                                0-get the calibration values unsuccessfully and Need to recalibrate
******************************************************************************/
u8 TP_Get_Adjdata(void)
{
    /*s32 tempfac;
    tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);//¶БИЎ±кјЗЧЦ,їґКЗ·сРЈЧј№эЈЎ
    if(tempfac==0X0A)//ґҐГюЖБТСѕ­РЈЧј№эБЛ
    {
        tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);
        tp_dev.xfac=(float)tempfac/100000000;//µГµЅxРЈЧјІОКэ
        tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);
        tp_dev.yfac=(float)tempfac/100000000;//µГµЅyРЈЧјІОКэ
        //µГµЅxЖ«ТЖБї
        tp_dev.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);
        //µГµЅyЖ«ТЖБї
        tp_dev.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);
        tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//¶БИЎґҐЖБАаРН±кјЗ
        if(tp_dev.touchtype)//X,Y·ЅПтУлЖБД»Па·ґ
        {
            CMD_RDX=0X90;
            CMD_RDY=0XD0;
        }else                  //X,Y·ЅПтУлЖБД»ПаН¬
        {
            CMD_RDX=0XD0;
            CMD_RDY=0X90;
        }
        return 1;
    }*/
    return 0;
}

//МбКѕЧЦ·ыґ®
const char* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";

/*****************************************************************************
 * @name       :void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
 * @date       :2018-08-09
 * @function   :Display calibration results
 * @parameters :x0:the x coordinates of first calibration point
                                y0:the y coordinates of first calibration point
                                x1:the x coordinates of second calibration point
                                y1:the y coordinates of second calibration point
                                x2:the x coordinates of third calibration point
                                y2:the y coordinates of third calibration point
                                x3:the x coordinates of fourth calibration point
                                y3:the y coordinates of fourth calibration point
                                fac:calibration factor
 * @retvalue   :None
******************************************************************************/
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{
/*    POINT_COLOR=RED;
    LCD_ShowString(40,140,16,"x1:",1);
    LCD_ShowString(40+80,140,16,"y1:",1);
    LCD_ShowString(40,160,16,"x2:",1);
    LCD_ShowString(40+80,160, 16,"y2:",1);
    LCD_ShowString(40,180, 16,"x3:",1);
    LCD_ShowString(40+80,180, 16,"y3:",1);
    LCD_ShowString(40,200, 16,"x4:",1);
    LCD_ShowString(40+80,200, 16,"y4:",1);
    LCD_ShowString(40,220, 16,"fac is:",1);
    LCD_ShowNum(40+24,140,x0,4,16);     //ПФКѕКэЦµ
    LCD_ShowNum(40+24+80,140,y0,4,16);  //ПФКѕКэЦµ
    LCD_ShowNum(40+24,160,x1,4,16);     //ПФКѕКэЦµ
    LCD_ShowNum(40+24+80,160,y1,4,16);  //ПФКѕКэЦµ
    LCD_ShowNum(40+24,180,x2,4,16);     //ПФКѕКэЦµ
    LCD_ShowNum(40+24+80,180,y2,4,16);  //ПФКѕКэЦµ
    LCD_ShowNum(40+24,200,x3,4,16);     //ПФКѕКэЦµ
    LCD_ShowNum(40+24+80,200,y3,4,16);  //ПФКѕКэЦµ
    LCD_ShowNum(40+56,220,fac,3,16);    //ПФКѕКэЦµ,ёГКэЦµ±ШРлФЪ95~105·¶О§Ц®ДЪ.*/
}

/*****************************************************************************
 * @name       :u8 TP_Get_Adjdata(void)
 * @date       :2018-08-09
 * @function   :Calibration touch screen and Get 4 calibration parameters
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TP_Adjust(void)
{
    /*u16 pos_temp[4][2];//Чш±к»єґжЦµ
    u8  cnt=0;
    u16 d1,d2;
    u32 tem1,tem2;
    double fac;
    u16 outtime=0;
    cnt=0;
    POINT_COLOR=BLUE;
    BACK_COLOR =WHITE;
    LCD_Clear(WHITE);//ЗеЖБ
    POINT_COLOR=RED;//ємЙ«
    LCD_Clear(WHITE);//ЗеЖБ
    POINT_COLOR=BLACK;
    LCD_ShowString(10,40,16,"Please use the stylus click",1);//ПФКѕМбКѕРЕПў
    LCD_ShowString(10,56,16,"the cross on the screen.",1);//ПФКѕМбКѕРЕПў
    LCD_ShowString(10,72,16,"The cross will always move",1);//ПФКѕМбКѕРЕПў
    LCD_ShowString(10,88,16,"until the screen adjustment",1);//ПФКѕМбКѕРЕПў
  LCD_ShowString(10,104,16,"is completed.",1);//ПФКѕМбКѕРЕПў

    TP_Drow_Touch_Point(20,20,RED);//»­µг1
    tp_dev.sta=0;//ПыіэґҐ·ўРЕєЕ
    tp_dev.xfac=0;//xfacУГАґ±кјЗКЗ·сРЈЧј№э,ЛщТФРЈЧјЦ®З°±ШРлЗеµф!ТФГвґнОу
    while(1)//Из№ыБ¬Рш10ГлЦУГ»УР°ґПВ,ФтЧФ¶ЇНЛіц
    {
        tp_dev.scan(1);//ЙЁГиОпАнЧш±к
        if((tp_dev.sta&0xc0)==TP_CATH_PRES)//°ґјь°ґПВБЛТ»ґО(ґЛК±°ґјьЛЙїЄБЛ.)
        {
            outtime=0;
            tp_dev.sta&=~(1<<6);//±кјЗ°ґјьТСѕ­±»ґ¦Ан№эБЛ.

            pos_temp[cnt][0]=tp_dev.x;
            pos_temp[cnt][1]=tp_dev.y;
            cnt++;
            switch(cnt)
            {
                case 1:
                    TP_Drow_Touch_Point(20,20,WHITE);               //Зеіэµг1
                    TP_Drow_Touch_Point(lcddev.width-20,20,RED);    //»­µг2
                    break;
                case 2:
                    TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);  //Зеіэµг2
                    TP_Drow_Touch_Point(20,lcddev.height-20,RED);   //»­µг3
                    break;
                case 3:
                    TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);         //Зеіэµг3
                    TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);  //»­µг4
                    break;
                case 4:  //И«ІїЛДёцµгТСѕ­µГµЅ
                    //¶Ф±ЯПаµИ
                    tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
                    tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
                    tem1*=tem1;
                    tem2*=tem2;
                    d1=sqrt(tem1+tem2);//µГµЅ1,2µДѕаАл

                    tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
                    tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
                    tem1*=tem1;
                    tem2*=tem2;
                    d2=sqrt(tem1+tem2);//µГµЅ3,4µДѕаАл
                    fac=(float)d1/d2;
                    if(fac<0.95||fac>1.05||d1==0||d2==0)//І»єПёс
                    {
                        cnt=0;
                        TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);    //Зеіэµг4
                        TP_Drow_Touch_Point(20,20,RED);                             //»­µг1
                        TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//ПФКѕКэѕЭ
                        continue;
                    }
                    tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
                    tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
                    tem1*=tem1;
                    tem2*=tem2;
                    d1=sqrt(tem1+tem2);//µГµЅ1,3µДѕаАл

                    tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
                    tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
                    tem1*=tem1;
                    tem2*=tem2;
                    d2=sqrt(tem1+tem2);//µГµЅ2,4µДѕаАл
                    fac=(float)d1/d2;
                    if(fac<0.95||fac>1.05)//І»єПёс
                    {
                        cnt=0;
                        TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);    //Зеіэµг4
                        TP_Drow_Touch_Point(20,20,RED);                             //»­µг1
                        TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//ПФКѕКэѕЭ
                        continue;
                    }//ХэИ·БЛ

                    //¶ФЅЗПЯПаµИ
                    tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
                    tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
                    tem1*=tem1;
                    tem2*=tem2;
                    d1=sqrt(tem1+tem2);//µГµЅ1,4µДѕаАл

                    tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
                    tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
                    tem1*=tem1;
                    tem2*=tem2;
                    d2=sqrt(tem1+tem2);//µГµЅ2,3µДѕаАл
                    fac=(float)d1/d2;
                    if(fac<0.95||fac>1.05)//І»єПёс
                    {
                        cnt=0;
                        TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);    //Зеіэµг4
                        TP_Drow_Touch_Point(20,20,RED);                             //»­µг1
                        TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//ПФКѕКэѕЭ
                        continue;
                    }//ХэИ·БЛ
                    //јЖЛгЅб№ы
                    tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);//µГµЅxfac
                    tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//µГµЅxoff

                    tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);//µГµЅyfac
                    tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//µГµЅyoff
                    if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)//ґҐЖБєНФ¤ЙиµДПа·ґБЛ.
                    {
                        cnt=0;
                        TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);    //Зеіэµг4
                        TP_Drow_Touch_Point(20,20,RED);                             //»­µг1
                        LCD_ShowString(40,26, 16,"TP Need readjust!",1);
                        tp_dev.touchtype=!tp_dev.touchtype;//РЮёДґҐЖБАаРН.
                        if(tp_dev.touchtype)//X,Y·ЅПтУлЖБД»Па·ґ
                        {
                            CMD_RDX=0X90;
                            CMD_RDY=0XD0;
                        }else                  //X,Y·ЅПтУлЖБД»ПаН¬
                        {
                            CMD_RDX=0XD0;
                            CMD_RDY=0X90;
                        }
                        continue;
                    }
                    POINT_COLOR=BLUE;
                    LCD_Clear(WHITE);//ЗеЖБ
                    LCD_ShowString(35,110, 16,"Touch Screen Adjust OK!",1);//РЈХэНкіЙ
                    delay_ms(1000);
                    TP_Save_Adjdata();
                    LCD_Clear(WHITE);//ЗеЖБ
                    return;//РЈХэНкіЙ
            }
        }
        delay_ms(10);
        outtime++;
        if(outtime>1000)
        {
            TP_Get_Adjdata();
            break;
        }
    }*/
}

/*****************************************************************************
 * @name       :u8 TP_Init(void)
 * @date       :2018-08-09
 * @function   :Initialization touch screen
 * @parameters :None
 * @retvalue   :0-no calibration
                                1-Has been calibrated
******************************************************************************/
u8 TP_Init(void)
{
    /*GPIO_InitTypeDef GPIO_InitStructure;    //GPIO
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);//К№ДЬGPIOB,C,FК±ЦУ
*/
    TP_Read_XY(&tp_dev.x,&tp_dev.y);//µЪТ»ґО¶БИЎіхКј»Ї
    //AT24CXX_Init();//іхКј»Ї24CXX
    if(TP_Get_Adjdata())return 0;//ТСѕ­РЈЧј
    else               //ОґРЈЧј?
    {
        //LCD_Clear(WHITE);//ЗеЖБ
        TP_Adjust();  //ЖБД»РЈЧј
        TP_Save_Adjdata();
    }
    TP_Get_Adjdata();
    return 1;
}

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

bool ILI9341_TouchGetCoordinates(uint16_t* x, uint16_t* y) {
    static const uint8_t cmd_read_x[] = { READ_X };
    static const uint8_t cmd_read_y[] = { READ_Y };
    static const uint8_t zeroes_tx[] = { 0x00, 0x00 };

    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
    uint8_t nsamples = 0;
    for(uint8_t i = 0; i < 16; i++) {
        volatile uint8_t p = PEN;
        if(p!=0) {
            //if(!ILI9341_TouchPressed())
            break;
        }
        nsamples++;

        HAL_SPI_Transmit(&ILI9341_TOUCH_SPI_PORT,
            (uint8_t*)cmd_read_y, sizeof(cmd_read_y), HAL_MAX_DELAY);
        uint8_t y_raw[2];
        HAL_SPI_TransmitReceive(&ILI9341_TOUCH_SPI_PORT,
            (uint8_t*)zeroes_tx, y_raw, sizeof(y_raw), HAL_MAX_DELAY);

        HAL_SPI_Transmit(&ILI9341_TOUCH_SPI_PORT,
            (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
        uint8_t x_raw[2];
        HAL_SPI_TransmitReceive(&ILI9341_TOUCH_SPI_PORT,
            (uint8_t*)zeroes_tx, x_raw, sizeof(x_raw), HAL_MAX_DELAY);

        avg_x += (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
        avg_y += (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
    }

    if(nsamples < 16)
        return false;

    uint32_t raw_x = (avg_x / 16);
    if(raw_x < ILI9341_TOUCH_MIN_RAW_X)
        raw_x = ILI9341_TOUCH_MIN_RAW_X;
    if(raw_x > ILI9341_TOUCH_MAX_RAW_X)
        raw_x = ILI9341_TOUCH_MAX_RAW_X;

    uint32_t raw_y = (avg_y / 16);
    if(raw_y < ILI9341_TOUCH_MIN_RAW_X)
        raw_y = ILI9341_TOUCH_MIN_RAW_Y;
    if(raw_y > ILI9341_TOUCH_MAX_RAW_Y)
        raw_y = ILI9341_TOUCH_MAX_RAW_Y;

    // Uncomment this line to calibrate touchscreen:
    // UART_Printf("raw_x = %d, raw_y = %d\r\n", x, y);

    *x = (raw_x - ILI9341_TOUCH_MIN_RAW_X) * ILI9341_TOUCH_SCALE_X /
         (ILI9341_TOUCH_MAX_RAW_X - ILI9341_TOUCH_MIN_RAW_X);
    *y = (raw_y - ILI9341_TOUCH_MIN_RAW_Y) * ILI9341_TOUCH_SCALE_Y /
         (ILI9341_TOUCH_MAX_RAW_Y - ILI9341_TOUCH_MIN_RAW_Y);

    return true;
}
