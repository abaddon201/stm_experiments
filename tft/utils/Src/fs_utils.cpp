/*
 * fs_utils.cpp
 *
 *  Created on: 26 сент. 2020 г.
 *      Author: user
 */

#include "fs_utils.h"

#include "base.h"

namespace {
const char *fileSystemFaults = "OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
        "INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
        "INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
        "LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
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

// Важно! ffconf.h : #define _FS_RPATH 0
bool ReadDir(char *dirPath, std::vector<FileInfo> &list) {
    DIR dir;
    FRESULT ret;
    FILINFO fno;

    ret = f_opendir(&dir, dirPath);   // Open a directory
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
        return false;
    }
    list.clear();
    //
    if (strcmp(dirPath, "/") != 0) {
        // Non-root directory must contain "<UP-DIR>"
        FileInfo dir("..", true);
        list.push_back(dir);
    }
    while (true) {
        ret = f_readdir(&dir, &fno);  // Read a directory item
        if (ret != FR_OK) {
            return false;
        }
        if (fno.fname[0] == 0) {
            break;  // Break on end of dir
        }
        bool isDir = fno.fattrib & AM_DIR;
        /*if (fno.fattrib & AM_RDO) {
         // Is Read-Only
         buffB[_MAX_LFN * i + 1] = 'R';
         } else {
         buffB[_MAX_LFN * i + 1] = ' ';
         }*/
        //buffB[_MAX_LFN * i + 2] = ' ';   // reserved
        // add filter
        FileInfo info(fno.fname, isDir);
        list.push_back(info);
    }
    f_closedir(&dir);
    return true;
}
