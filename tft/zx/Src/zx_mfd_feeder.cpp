/*
 * zx_mfd_feeder.cpp
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Abby
 */

#include "zx_mfd_feeder.h"

#include <string.h>

#include "base.h"
#include "fs_utils.h"
#include "zx_disk_feeder.h"

 /*extern char sFileFullNameA[];
 extern bool bFileLoadedA;
 */
#define BITRATE1 0x00FA
#define BITRATE2 0x01F4


MFDIMG* mfdimg;
MFDTRACKIMG* mfdtrackimg;
uint16_t mfd_NOTA;              // общее кол-во треков в файле ( с учетом сторон )

//FIXME: provide common error handling for project
extern void xprintf(const char* msg);

bool ZxMfdFeeder::parseImageFile(const char* fileName) {
    UINT br;
    Unsaved_CylinderA = false;

    FRESULT ret;

    FILINFO fno; /* File information object */

    ret = f_stat(fileName, &fno);
    if (ret) {
        return false;
    }
    // Если файл Read-Only, то ставим fWRPT
    d_isReadonly = fno.fattrib & AM_RDO;
    // FIXME: read/write access
    ret = f_open(&file, fileName, FA_READ);
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
    }
    //
    ret = f_read(&file, buffA, 0x1020, &br);  // 0x1020 - заголовки + таблица смещений треков
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
    }
    //
    /*ret = f_close(&file);
     if (ret)
     fault_err (ret);*/
     //
    mfdimg = (MFDIMG*) buffA;
    //
    if (strcmp("MFMDECD", (char const*) mfdimg->headername)) {
        xprintf("Wrong header");
        return false;
    }
    if (mfdimg->max_cylinder > CYL_NUM_MAX) {
        xprintf("Tracks > MAX");
        return false;
    }
    if (mfdimg->max_side > 1) {
        xprintf("Sides > 2");
        return false;
    }
    if ((mfdimg->bit_rate != BITRATE1) && (mfdimg->bit_rate != BITRATE2)) {
        xprintf("Neither 250 nor 500 kbs");
        return false;
    }
    //
    mfd_NOTA = (mfdimg->max_cylinder + 1) * (mfdimg->max_side + 1);
    mfdtrackimg = (MFDTRACKIMG*) (buffA + sizeof(MFDIMG));
    //
    for (int i = 0; i < mfd_NOTA; i++) {
        track_lengsA[i] = mfdtrackimg->mfd_tracksize;
        track_offsetsA[i] = mfdtrackimg->mfd_trackoffset;
        mfdtrackimg++;
    }
    //
    Max_CylinderA = mfdimg->max_cylinder;
    //
    return true;
}

void ZxMfdFeeder::loadCylinder(uint32_t Cyl_Number) {
    FRESULT ret;
    UINT br;
    //  if ( Cyl_Number > Max_Cylinder )    return;
    //pCTRL->BSRRL = EM_TST;
        //
        /*ret = f_open(&file, sFileFullNameA, FA_READ);
         if (ret)
         fault_err(ret);*/
         //
    ret = f_lseek(&file, track_offsetsA[Cyl_Number * 2]);
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
    }
    //
    ret = f_read(&file, &buffA, (MAX_TRACK_LEN + MAX_BF_LEN) << 1, &br);
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
    }
    //
    /*ret = f_close(&file);
     if (ret)
     fault_err(ret);*/
     //
    Loaded_CylinderA = Cyl_Number;
    //
//pCTRL->BSRRH = EM_TST;
}

void ZxMfdFeeder::saveCylinder() {
    FRESULT ret;
    UINT bw;
    //  if ( Cyl_Number > Max_Cylinder )
    //pCTRL->BSRRL = EM_TST;
        //
        /*ret = f_open(&file, sFileFullNameA, FA_WRITE);
         if (ret)
         fault_err(ret);*/
         //
    ret = f_lseek(&file, track_offsetsA[Loaded_CylinderA * 2]);
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
    }
    //
    ret = f_write(&file, &buffA, (MAX_TRACK_LEN + MAX_BF_LEN) << 1, &bw);
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
    }
    //
//xprintf("written=%d, ret=%d\r\n", bw, ret );
    //
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
    }
    //
    /*ret = f_close(&file);
     if (ret)
     fault_err(ret);*/
     //
    Unsaved_CylinderA = false;
    //
//pCTRL->BSRRH = EM_TST;
}

bool ZxMfdFeeder::init() {
    return true;
}
