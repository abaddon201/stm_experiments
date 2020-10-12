/*
 * zx_trd_feeder.cpp
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Abby
 */

#include "zx_trd_feeder.h"

#include <string.h>

#include "base.h"
#include "fs_utils.h"
#include "crc16.h"
#include "zx_disk_feeder.h"

#define BITRATE1 0x00FA
#define BITRATE2 0x01F4

//FIXME: provide common error handling for project
extern void xprintf(const char *msg);

#define GAP0_SIZE   10
#define GAP0_VALUE  0x4E
#define ZERO0_SIZE   12
#define ZERO 0
#define IAM_SIZE 7

#define GAP2_SIZE    22
#define GAP2_VALUE  0x4E

#define GAP3_SIZE    60
#define GAP3_VALUE  0x4E

#define GAP4_SIZE    214
#define GAP4_VALUE  0x4E

#define ZERO1_SIZE   128

#define SYNC_SIZE 3
#define SYNC_VALUE    0xA1

#define IAM1    0xFE
#define IDAM1   0xFB

#define DATA_OFFSET (GAP0_SIZE + ZERO0_SIZE +SYNC_SIZE +IAM_SIZE+GAP2_SIZE +ZERO0_SIZE+SYNC_SIZE+1)

#define FULL_SIZE (ZERO0_SIZE+SYNC_SIZE+IAM_SIZE+GAP2_SIZE+ZERO0_SIZE+SYNC_SIZE+1+256+GAP3_SIZE)

bool ZxTrdFeeder::parseImageFile(const char *fileName) {
    //UINT br;
    Unsaved_CylinderA = false;
    uint8_t *crc_bufptr;
    FRESULT ret;
    FILINFO fno; /* File information object */
    void *BF_Addr;
    unsigned char BF_BitNum;

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
    uint8_t *bufptr = buffA;
    memset(bufptr, GAP0_VALUE, GAP0_SIZE);
    bufptr += GAP0_SIZE;
    for (int j = 1; j <= 16; j++) {
        memset(bufptr, ZERO, ZERO0_SIZE);
        bufptr += ZERO0_SIZE;
        crc_bufptr = bufptr;
        memset(bufptr, SYNC_VALUE, SYNC_SIZE);
        BF_Addr = (void*) (((unsigned long) (bufptr - buffA) >> 3) + (unsigned long) buffA + MAX_TRACK_LEN);
        BF_BitNum = (char) ((bufptr - buffA) & 7);
        *(char*) BF_Addr |= 1 << BF_BitNum;
        bufptr += SYNC_SIZE;
        *(char*) bufptr++ = IAM1;
        bufptr++;
        //*(char*) bufptr++ = (char) (i >> 1); //GAP0_SIZE+ZERO0_SIZE+SYNC_SIZE+1
        *(char*) bufptr++ = (char) 0; // TR-DOS marks all sides 0
//          *bufptr++ = (char)( i & 1 );
        *(char*) bufptr++ = j;
        *(char*) bufptr++ = 0x01;    // 256 bytes/sector
        uint16_t crc = crc16(crc_bufptr, 8);
        *bufptr++ = crc >> 8;
        *bufptr++ = crc & 0xFF;
        memset(bufptr, GAP2_VALUE, GAP2_SIZE);
        bufptr += GAP2_SIZE;
        memset(bufptr, ZERO, ZERO0_SIZE);
        bufptr += ZERO0_SIZE;
        memset(bufptr, SYNC_VALUE, SYNC_SIZE);
        BF_Addr = (void*) (((unsigned long) (bufptr - buffA) >> 3) + (unsigned long) buffA + MAX_TRACK_LEN);
        BF_BitNum = (char) ((bufptr - buffA) & 7);
        *(char*) BF_Addr |= 1 << BF_BitNum;
        //crc_bufptr = bufptr;
        bufptr += SYNC_SIZE;
        *(char*) bufptr++ = IDAM1;
        //sector data
        bufptr += 256;
        memset(bufptr, GAP3_VALUE, GAP3_SIZE);
        bufptr += GAP3_SIZE;
    }
    memset(bufptr, GAP4_VALUE, GAP4_SIZE);
    bufptr += GAP4_SIZE;
    memset(bufptr, ZERO, ZERO1_SIZE);  // fill all the buffer (6400 bytes)

    return true;
}

void ZxTrdFeeder::loadCylinder(uint32_t Cyl_Number) {
    FRESULT ret;
    UINT br;
//  if ( Cyl_Number > Max_Cylinder )    return;
    //
    ret = f_lseek(&file, Cyl_Number * 16 * 256 * 2);
    if (ret) {
        baseErrorHandler(fileSystemFault(ret));
    }
    //
    for (int i = 0; i < 16; ++i) {
        buffA[GAP0_SIZE + ZERO0_SIZE + SYNC_SIZE + 1 + i * FULL_SIZE] = (uint8_t) (Cyl_Number >> 1);
        uint8_t *bufptr = &(buffA[DATA_OFFSET + i * FULL_SIZE]);
        uint8_t *crc_bufptr = bufptr - SYNC_SIZE;
        ret = f_read(&file, bufptr, 256, &br);
        if (ret) {
            baseErrorHandler(fileSystemFault(ret));
        }
        bufptr += 256;
        uint16_t crc = crc16(crc_bufptr, 260);
        *bufptr++ = crc >> 8;
        *bufptr++ = crc & 0xFF;
    }
    Loaded_CylinderA = Cyl_Number;
}

void ZxTrdFeeder::saveCylinder() {
    FRESULT ret;
    UINT bw;
//  if ( Cyl_Number > Max_Cylinder )
    for (int i = 0; i < 16; ++i) {
        uint8_t *bufptr = &(buffA[DATA_OFFSET + i * FULL_SIZE]);
        ret = f_lseek(&file, Loaded_CylinderA * 16 * 256 * 2);
        if (ret) {
            baseErrorHandler(fileSystemFault(ret));
        }
        //
        ret = f_write(&file, bufptr, (16 * 256) << 1, &bw);
        if (ret) {
            baseErrorHandler(fileSystemFault(ret));
        }
    }
    //
//xprintf("written=%d, ret=%d\r\n", bw, ret );
    Unsaved_CylinderA = false;
}

bool ZxTrdFeeder::init() {
    return true;
}
