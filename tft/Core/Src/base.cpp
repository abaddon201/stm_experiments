#include "cmsis_os.h"
#include "dac.h"
#include "fatfs.h"
#include "sdio.h"
#include "gpio.h"
#include "fsmc.h"
#include "fonts.h"

#include "ili9341.h"

#include <string.h>

#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)

FontSystem fonts;

extern "C" FATFS SDFatFS; /* File system object for SD card logical drive */

void displayTask(void *params) {
    char SD_Path[4]; /* SD logical drive path */

    FRESULT res; /* FatFs function common result code */
    memset(&SDFatFS, 0, sizeof(FATFS));
    res = f_mount(&SDFatFS, (TCHAR const*) SD_Path, 1);
    if (res != FR_OK) {
        Error_Handler();
        /*    } else {
         HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);*/
    }
    fonts.init();
    /*uint32_t start;
     uint32_t stop;
     uint32_t delta;*/
    TFT9341_FillScreen(RED);
    while (1) {
        //start = ARM_CM_DWT_CYCCNT;
        //TFT9341_FillScreen(BLUE);
        fonts.drawString(&fonts.Font16, 10, 10, "Hello world");
        /*stop  = ARM_CM_DWT_CYCCNT;
         delta = stop-start;
         stop=0;*/
    }
}

extern "C" void baseInit() {
    TFT9341_ini();

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

    xTaskCreate(displayTask, "Display", 1024, 0, 1, 0);
}
