/*
 * zx_mfd_feeder.h
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_ZX_MFD_FEEDER_H_
#define INC_ZX_MFD_FEEDER_H_

#include <stdio.h>

#include "../../_zx/Inc/zx_disk_feeder.h"

// Заголовок файла образа
typedef struct {
    unsigned char headername[8];    // "MFMDECD",0
    unsigned char max_cylinder; // 0..255
    unsigned char max_side;     // 0..1
    unsigned short bit_rate;        // 0x00FA -> 250e+3
    unsigned char reserved[20];
} __attribute__((packed)) MFDIMG;   // 32 байт

// Блок описания дорожек
typedef struct {
    unsigned char cylinder_number;
    unsigned char side_number;
    unsigned short mfd_tracksize;   // в байтах, не более MAX_TRACK_LEN
    unsigned long mfd_trackoffset;  // смещение (в файле) блока данных дорожки
} __attribute__((packed)) MFDTRACKIMG;  // 8 байт * ( max_cylinder + 1 ) * ( max_side + 1)

class ZxMfdFeeder: public ZxDiskFeeder {
public:
    bool parseImageFile(const char* fileName) override;

    void loadCylinder(uint32_t Cyl_Number) override;
    void saveCylinder() override;

    bool init() override;

};
#endif /* INC_ZX_MFD_FEEDER_H_ */
