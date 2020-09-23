#include "base.h"

#include "zx_tap_player.h"
#include "zx_tap_feeder.h"

#include "task_display.h"

#include <string.h>

ZxTapPlayer zxTapPlayer;
ZxTapFeeder zxTapFeeder;

extern "C" FATFS SDFatFS; /* File system object for SD card logical drive */
//extern "C" DMA_HandleTypeDef hdma_dac1;

extern "C" void baseInit() {

    xTaskCreate(displayTask, "Display", 3024, 0, 1, 0);

    //FIXME: think where to move it
    zxTapFeeder.initFile("Batty.tap", 44100);
    zxTapPlayer.play(&zxTapFeeder);

}

