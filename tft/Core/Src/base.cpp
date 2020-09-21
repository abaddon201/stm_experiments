#include "cmsis_os.h"
#include "dac.h"
#include "fatfs.h"
#include "sdio.h"
#include "gpio.h"
#include "fsmc.h"
#include "fonts.h"

#include "ili9341.h"

#include "touch.h"
#include "zx_tap_player.h"
#include "zx_tap_feeder.h"

#include <string.h>

#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)

FontSystem *fonts;
Tft9341 *display;
ZxTapPlayer zxTapPlayer;
ZxTapFeeder zxTapFeeder;

extern "C" FATFS SDFatFS; /* File system object for SD card logical drive */
extern "C" DMA_HandleTypeDef hdma_dac1;

static void delay_ms(__IO uint32_t micros) {
    micros *= (SystemCoreClock / 1000000) / 5;

    while (micros--)
        ;
}

void displayTask(void *params) {
    display->init();
    char SD_Path[4]; /* SD logical drive path */

    FRESULT res; /* FatFs function common result code */
    memset(&SDFatFS, 0, sizeof(FATFS));
    res = f_mount(&SDFatFS, (TCHAR const*) SD_Path, 1);
    if (res != FR_OK) {
        Error_Handler();
        /*    } else {
         HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);*/
    }
    fonts->init();

    zxTapFeeder.initFile("Batty.tap", 44100);
    zxTapPlayer.play(&zxTapFeeder);
    //TP_Init();
    /*uint32_t start;
     uint32_t stop;
     uint32_t delta;*/
    uint16_t x,y;
    uint8_t pressed;
    while (1) {
        //TFT9341_FillScreen(RED);
        //start = ARM_CM_DWT_CYCCNT;
        //TFT9341_FillScreen(BLUE);
        //fonts->drawString(&fonts->Font16, 0, 62, "Hello world");
//        pressed=TP_Read_XY(&x, &y);
        //pressed = ILI9341_TouchGetCoordinates(&x, &y);

        /*stop  = ARM_CM_DWT_CYCCNT;
         delta = stop-start;
         stop=0;*/
        //zxTapPlayer.playWave(2047);
        delay_ms(489);
        //zxTapPlayer.playWave(1023);
        delay_ms(245);
    }
}

extern "C" void baseInit() {
    //zxTapPlayer.playWave(807);
    HAL_Delay(2000);
    display = new Tft9341();
    fonts = new FontSystem(display);

    if (ARM_CM_DWT_CTRL != 0) {        // See if DWT is available
        ARM_CM_DEMCR |= 1 << 24;  // Set bit 24
        ARM_CM_DWT_CYCCNT = 0;
        ARM_CM_DWT_CTRL |= 1 << 0;   // Set bit 0
    }

    /*for(i=0;i<200;++i){
     start = ARM_CM_DWT_CYCCNT;
     stop  = ARM_CM_DWT_CYCCNT;

     delta = stop - start;
     ms = delta*1000/168000000;
     }*/

    xTaskCreate(displayTask, "Display", 3024, 0, 1, 0);
}

