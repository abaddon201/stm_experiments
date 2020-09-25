#include "base.h"

#include <string.h>
#include <fatfs.h>

#include "config_file.h"
#include "task_display.h"
#include "zx_tap_feeder.h"
#include "zx_tap_player.h"

ZxTapPlayer zxTapPlayer;
ZxTapFeeder zxTapFeeder;

namespace {
const char *fileSystemFaults = "OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
        "INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
        "INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
        "LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
}

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

extern "C" void baseErrorHandler(const char *msg) {

}

const char* fileSystemFault(FRESULT rc) {
    const char *str = fileSystemFaults;
    int i;

    for (i = 0; i != rc && *str; ++i) {
        while (*str++)
            ;
    }
    return str;
    /*    xprintf((const char*) str);
     #ifdef ERROR_RESET
     Delay(75);
     NVIC_SystemReset();
     #else*/
    /*    while (1)
     ;*/
    //#endif
}

