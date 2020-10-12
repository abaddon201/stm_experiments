/*
 * fs_utils.h
 *
 *  Created on: 26 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_FS_UTILS_H_
#define INC_FS_UTILS_H_

#include <string.h>
#include <vector>

#include "ff.h"

struct FileInfo {
    char *name;
    bool isDir;

    FileInfo(const char *name, bool isDir) {
        int len = strlen(name);
        this->name = new char[len];
        this->isDir = isDir;
        strcpy(this->name, name);
    }

    ~FileInfo() {
        delete[] name;
    }
};

const char* fileSystemFault(FRESULT rc);

bool ReadDir(char *dirPath, std::vector<FileInfo> &list);

#endif /* INC_FS_UTILS_H_ */
