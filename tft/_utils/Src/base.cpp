#include "../../_utils/Inc/base.h"

#include <string.h>
#include <fatfs.h>

#include "../../_utils/Inc/config_file.h"
#include "../../_utils/Inc/config_file.h"
#include "../../_video/Inc/task_display.h"
#include "../../_zx/Inc/zx_tap_feeder.h"
#include "../../_zx/Inc/zx_tap_player.h"

ZxTapPlayer zxTapPlayer;
ZxTapFeeder zxTapFeeder;

extern "C" void baseInit() {
    char SD_Path[4]; /* SD logical drive path */

    FRESULT res; /* FatFs function common result code */
    memset(&SDFatFS, 0, sizeof(FATFS));
    res = f_mount(&SDFatFS, (TCHAR const*) SD_Path, 1);
    if (res != FR_OK) {
        Error_Handler();
    }

    config.load();
    xTaskCreate(displayTask, "Display", 3024, 0, 1, 0);

    //FIXME: think where to move it
    zxTapFeeder.initFile("Batty.tap", 44100);
    zxTapPlayer.play(&zxTapFeeder);
}
