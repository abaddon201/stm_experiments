/*
 * zx_vg93.h
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_ZX_VG93_H_
#define INC_ZX_VG93_H_

#include <stdio.h>

#include "../../_zx/Inc/zx_disk_feeder.h"

#define VERSION "v0.05"

#define pCTRL GPIOB

#define DATA_AO8 (*(__IO uint8_t *)((uint32_t)&(GPIOA->ODR)))
#define DATA_I8 (*(__IO uint8_t *)((uint32_t)&(GPIOA->IDR)))

// FIXME: need to check pin bits
#define EM_A0 ((uint32_t) 1<<0 )    // PB0
#define EM_A1 ((uint32_t) 1<<1 )    // PB1
#define EM_A01 ((uint32_t)( EM_A0 | EM_A1 ))    // PB0,1
#define EM_RESET ((uint32_t) 1<<2 ) // PB2
#define EM_HLT ((uint32_t) 1<<13 )  // PB13
#define EM_SIDE ((uint32_t) 1<<14 ) // PB14
#define EM_DISK_NUM ((uint32_t) 1<<15 ) // PB15

#define EM_DRQ      (1<<6 ) // PB6
#define EM_INTRQ    (1<<7 ) // PB7

#define EM_TST ((uint16_t) 1<<8 )   // PB8
#define EM_TST2 ((uint16_t) 1<<9 )  // PB9
#define EM_TST3 ((uint16_t) 1<<11 ) // PB11

#define INDEX_START 0
#define INDEX_STOP 63

#define MAX_PATH_LEN 255
#define FILE_NAME_LEN 16
// 1 символ(' ','*') + 1 символ (' ','R') + 12 длина(8.3) + 1 завершающий '0' = 15
#define MAX_CON_LINES 13

#define IDAM 0xF8A1A1A1
#define IAM 0xFEA1A1A1

// Регистр статуса (для команд I типа)
#define BUSY        ((uint8_t) 1<<0 )
#define INDEX       ((uint8_t) 1<<1 )
#define TR00        ((uint8_t) 1<<2 )
#define CRC_ERR     ((uint8_t) 1<<3 )
#define SEEK_ERR    ((uint8_t) 1<<4 )
#define HLD_HLT     ((uint8_t) 1<<5 )
#define WRPT        ((uint8_t) 1<<6 )
#define CPRDY       ((uint8_t) 1<<7 )
// Регистр команд I типа
#define CMDI_R0     ((uint8_t) 1<<0 )
#define CMDI_R1     ((uint8_t) 1<<1 )
#define CMDI_R01    (CMDI_R0 | CMDI_R1)
#define CMDI_V      ((uint8_t) 1<<2 )
#define CMDI_h      ((uint8_t) 1<<3 )
#define CMD4        ((uint8_t) 1<<4 )
#define CMDI_u      CMD4
#define CMD5        ((uint8_t) 1<<5 )
#define CMD6        ((uint8_t) 1<<6 )
#define CMD7        ((uint8_t) 1<<7 )
#define CMD765      (CMD5 | CMD6 | CMD7)

// Регистр статуса (для команд II типа)
#define DRQ         ((uint8_t) 1<<1 )
#define LOST        ((uint8_t) 1<<2 )
#define REC_NF      ((uint8_t) 1<<4 )
#define WFAULT      ((uint8_t) 1<<5 )
#define REC_TYP     ((uint8_t) 1<<5 )

// Регистр команд II типа
#define CMDII_A0    ((uint8_t) 1<<0 )
#define CMDII_C     ((uint8_t) 1<<1 )
#define CMDII_E     ((uint8_t) 1<<2 )
#define CMDII_s     ((uint8_t) 1<<3 )
#define CMDII_m     ((uint8_t) 1<<4 )

// Регистр команд IV типа
#define CMDIV_I0    ((uint8_t) 1<<0 )
#define CMDIV_I1    ((uint8_t) 1<<1 )
#define CMDIV_I2    ((uint8_t) 1<<2 )
#define CMDIV_I3    ((uint8_t) 1<<3 )
#define CMDIV_I03   (CMDIV_I0 | CMDIV_I1 | CMDIV_I2 | CMDIV_I3)

#define min(a,b)  ((a) < (b) ? (a) : (b))

class ZxVg93 {
    ZxDiskFeeder *disk;
    void pickNLoad();
    void initializeEM();

    void execAccurateTypeI();
    void execAccurateTypeII();
    void execAccurateTypeIII();
    void execAccurateTypeIV();

    void dataTimerConfig();
    void dataTimerStart();
    void dataTimerContinue();
    void dataTimerStop();
    void delayConfig();
    void delayStart(uint16_t TIM_ARR);

    bool parseImageFileB();
    bool parseImageFileA();
    void loadCylinderA(uint32_t Cyl_Number);
    void saveCylinderA();
public:
    void mainLoop();
};

#endif /* INC_ZX_VG93_H_ */
