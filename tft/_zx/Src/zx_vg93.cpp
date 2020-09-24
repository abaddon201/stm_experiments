/*
 * zx_vg93.cpp
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Abby
 */

#include "../../_zx/Inc/zx_vg93.h"

#include <string.h>
#include <stdbool.h>

#include "ff.h"

#include "../../_utils/Inc/crc16.h"
#include "../../_zx/Inc/zx_disk_feeder.h"

 //const uint16_t R0R1_ARR[] = { 600, 1200, 2000, 3000 };
const uint16_t R0R1_ARR[] = {1, 600, 1400, 2400}; // коррекция на выполнение Load_Cylinder (6-7 мс)

#pragma pack(1)
volatile uint8_t Regs[4];
volatile uint8_t CMD_R;
#pragma pack()

volatile uint8_t DSR;
volatile uint8_t VTRACK_R;
extern volatile bool bNewCMD;
extern volatile uint8_t* ADDRPointer;

#define STAT_R  Regs[0]
#define TRACK_R Regs[1]
#define SECT_R  Regs[2]
#define DATA_R  Regs[3]

// Private variables

volatile bool HLD;

volatile uint8_t fBusy;     // 0
volatile uint8_t fIndex;    // 1
volatile uint8_t fTR00;     // 2
volatile uint8_t fCRC_ERR;  // 3
volatile uint8_t fSEEK_ERR; // 4
volatile uint8_t fHLD_HLT;  // 5
volatile uint8_t fWRPT;     // 6

volatile uint8_t fDRQ;      // 1
volatile uint8_t fLOST;     // 2
volatile uint8_t fREC_NF;   // 4
volatile uint8_t fWFAULT;   // 5
volatile uint8_t fREC_TYP;  // 5

/*uint8_t Max_CylinderB;
 bool Unsaved_CylinderB;*/
volatile bool PortReset;
long int i, j;
uint8_t CRCL_Saved;
volatile unsigned char CURR_STATE, LAST_STATE;
uint8_t CMD_TYPE;
uint16_t Sec_Length;
signed char DIRC;   // 1 или -1
volatile bool Wr_Sync;
volatile uint32_t* DWPointer;
volatile uint8_t* SYNCPointer;
volatile uint8_t* ADDRPointer;
volatile uint32_t temp2;
volatile bool INT_CMD;
uint8_t PREV_DATA;
char cRecUART;

extern volatile uint32_t Curr_Byte;
extern volatile uint8_t IP_Counter;
extern volatile bool Next_Byte;

void xprintf(const char* msg) {
    //TODO: log to somewhere
}

void vg93Task(void* params) {
    ZxVg93 zxVg93{ };
    zxVg93.mainLoop();
}

void ZxVg93::mainLoop() {
    STAT_R = 0;
    //strcpy(sCurrDir, "/");
    pickNLoad();
    while (1) {
        if (PortReset) {
            initializeEM();
        }
        //
        if (bNewCMD) {
            bNewCMD = false;
            if ((CMD_R & CMD7) == 0) {
                CMD_TYPE = 1;
            } else if ((CMD_R & CMD6) == 0) {
                CMD_TYPE = 2;
            } else if ((CMD_R & (CMD5 | CMD4)) != CMD4) {
                CMD_TYPE = 3;
            } else {
                CMD_TYPE = 4;
            }
            CURR_STATE = 1;
        }
        //
        switch (CMD_TYPE) {
        case 0:
            // остановка после 15 оборотов в IDLE
            if (IP_Counter >= 15) {
                dataTimerStop();
            }
            if (!HLD && !disk->unsavedCylinder()) {
                //if (ReceiveCharFromUSART(&cRecUART) && cRecUART == '\r')
                pickNLoad();
            }
            break;

        case 1:
            if ((pCTRL->IDR & EM_DISK_NUM) == 0) {
                execAccurateTypeI();
            }
            //              else
            //                  ExecFast_TypeI();
                        //
            if (VTRACK_R == 0) {
                fTR00 = TR00;
            } else {
                fTR00 = 0;
            }
            //
            STAT_R = fBusy | fIndex | fTR00 | fCRC_ERR | fSEEK_ERR | fHLD_HLT | fWRPT;
            break;

        case 2:
            if ((pCTRL->IDR & EM_DISK_NUM) == 0) {
                execAccurateTypeII();
            }
            //              else
            //                  ExecFast_TypeII();
            STAT_R = fBusy | fDRQ | fLOST | fCRC_ERR | fREC_NF | fWFAULT | fREC_TYP | fWRPT;
            break;

        case 3:
            if ((pCTRL->IDR & EM_DISK_NUM) == 0) {
                execAccurateTypeIII();
            }
            //              else
            //                  ExecFast_TypeIII();
            STAT_R = fBusy | fDRQ | fLOST | fCRC_ERR | fREC_NF | fWFAULT | fREC_TYP | fWRPT;
            break;

        case 4:
            if ((pCTRL->IDR & EM_DISK_NUM) == 0) {
                execAccurateTypeIV();
            }
            //              else
            //                  ExecFast_TypeIV();
            STAT_R = fBusy | fDRQ | fLOST | fCRC_ERR | fREC_NF | fWFAULT | fREC_TYP | fWRPT;
            break;

        default: break;
        }
    }
}

void ZxVg93::pickNLoad() {
    long int i = 0;

    while (i == 0) {
        //        ChooseDiskImage();
                //FIXME: provide name from UI
        if (disk->parseImageFile("test.mfd")) {
            if (disk->isReadonly()) {
                fWRPT = WRPT;
            } else {
                fWRPT = 0;
            }
            i++;
        }
    }
    initializeEM();
}

void ZxVg93::initializeEM() {
    PortReset = false;
    dataTimerStop();
    //
    fBusy = 0;
    fIndex = 0;
    fTR00 = 0;
    fCRC_ERR = 0;
    fSEEK_ERR = 0;
    fHLD_HLT = 0;
    fWRPT = 0;
    //
    fDRQ = 0;
    fLOST = 0;
    fREC_NF = 0;
    fWFAULT = 0;
    fREC_TYP = 0;
    //
    DIRC = -1;
    CMD_R = 0x03;
    SECT_R = 1;
    DATA_R = 0;
    TRACK_R = 0;
    VTRACK_R = 0;
    CMD_TYPE = 1;
    CURR_STATE = 1;
    bNewCMD = false;
    //
    while ((pCTRL->IDR & EM_RESET) == 0)
        ;
}

void ZxVg93::execAccurateTypeI() {
    switch (CURR_STATE) {
    case 1:
        fBusy = BUSY;
        fCRC_ERR = 0;
        fSEEK_ERR = 0;
        pCTRL->BSRR = EM_DRQ | EM_INTRQ;   // сброс
        CURR_STATE++;
        break;

    case 2:
        if (CMD_R & CMDI_h) {
            if (!HLD) {
                dataTimerStart();
            } else {
                dataTimerContinue();
            }
        }
        if (pCTRL->IDR & EM_HLT) {
            fHLD_HLT = HLD_HLT;
        } else {
            fHLD_HLT = 0;
        }
        CURR_STATE++;
        break;

    case 3:
        if ((CMD_R & CMD765) == 0x40) {
            // STEP-IN (Шаг вперед)
            DIRC = 1;
            CURR_STATE = 20;
        } else if ((CMD_R & CMD765) == 0x60) {
            // STEP-OUT (Шаг назад)
            DIRC = -1;
            CURR_STATE = 20;
        } else if ((CMD_R & CMD765) == 0x20) {
            // STEP (Шаг)
            CURR_STATE = 20;
        } else {
            CURR_STATE++;
        }
        break;

    case 4:
        if ((CMD_R & 0xF0) == 0) {
            // not SEEK (не Поиск)
            if (TRACK_R <= disk->maxCylinder() && TRACK_R >= 0) {
                VTRACK_R = TRACK_R;
            } else {
                VTRACK_R = 0;
            }
            TRACK_R = 0xFF;
            DATA_R = 0;
        }
        CURR_STATE++;
        break;

    case 5:
        DSR = DATA_R;
        CURR_STATE++;
        break;

    case 6:
        if (TRACK_R == DSR) {
            CURR_STATE = 12;
        } else {
            if (DSR > TRACK_R) {
                DIRC = 1;
            } else {
                DIRC = -1;
            }
            CURR_STATE++;
        }
        break;

    case 7:
        // B
        TRACK_R += DIRC;
        if (DIRC == 1 && VTRACK_R == disk->maxCylinder()) {
            ;
        } else if (DIRC == -1 && VTRACK_R == 0) {
            ;
        } else {
            VTRACK_R += DIRC;
        }
        CURR_STATE++;
        break;

    case 8:
        // C
        if (VTRACK_R == 0 && DIRC == -1) {
            //              fTR00 == TR00;
            TRACK_R = 0;
            CURR_STATE = 12;
        } else {
            CURR_STATE++;
        }
        break;

    case 9:
        delayStart(R0R1_ARR[CMD_R & CMDI_R01]);
        CURR_STATE++;
        break;

    case 10:
        if ((TIM6->CR1 & TIM_CR1_CEN) == 0) {
            CURR_STATE++;
        }
        break;

    case 11:
        if (CMD_R & CMD765) {
            // STEP-IN (Шаг вперед) или STEP-OUT (Шаг назад) или STEP (Шаг)
            CURR_STATE++;
        } else {
            CURR_STATE = 5;
        }
        break;

    case 12:
        if (disk->unsavedCylinder()) {
            disk->saveCylinder();
        }
        disk->loadCylinder(VTRACK_R);
        if (CMD_R & CMDI_V) {
            CURR_STATE++;
        } else {
            CURR_STATE = 19;
        }
        break;

    case 13:
        if (!HLD) {
            dataTimerStart();
        }
        //            else
        //               IP_Counter = 0;
        delayStart(3000);// при тактовой 1 Мгц
        CURR_STATE++;
        break;

    case 14:
        if ((TIM6->CR1 & TIM_CR1_CEN) == 0) {
            CURR_STATE++;
        }
        break;

    case 15:
        IP_Counter = 0;
        CURR_STATE++;
        break;

    case 16:
        if (IP_Counter > 5) {
            fSEEK_ERR = SEEK_ERR;
            CURR_STATE = 19;
        } else {
            CURR_STATE++;
        }
        break;

    case 17:
        if (Next_Byte) {
            Next_Byte = false;
            temp2 = Curr_Byte;
            SYNCPointer = disk->buffer() + MAX_TRACK_LEN + (Curr_Byte >> 3);
            if ((pCTRL->IDR & EM_SIDE) == 0) {
                temp2 += MAX_TRACK_LEN + MAX_BF_LEN;
                SYNCPointer += MAX_TRACK_LEN + MAX_BF_LEN;
            }
            DWPointer = (uint32_t*) (disk->buffer() + temp2);
            if (*DWPointer == IAM && (*SYNCPointer & 1 << (Curr_Byte & 7)) != 0) {
                if (*((uint8_t*) DWPointer + 4) == TRACK_R) {
                    CRCReset = true;
                    crc16(0xA1);
                    ADDRPointer = (uint8_t*) DWPointer + 1;
                    CURR_STATE++;
                }
            }
        }
        break;

    case 18:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 8) {
                if (*ADDRPointer == (uint8_t) (CRC16_value >> 8)
                    && *(++ADDRPointer) == (uint8_t) (CRC16_value & 0x00FF)) {
                    fCRC_ERR = 0;
                    CURR_STATE++;
                } else {
                    fCRC_ERR = CRC_ERR;
                    CURR_STATE = 16;
                }
            } else {
                crc16(*(ADDRPointer++));
            }
        }
        break;

    case 19:
        fBusy = 0;
        pCTRL->BSRR = EM_INTRQ;    // установка
        CURR_STATE = 0;
        CMD_TYPE = 0;
        break;

    case 20:
        if (CMD_R & CMDI_u) {
            CURR_STATE = 7;
        } else {
            CURR_STATE = 8;
        }
        break;

    default:
        break;
    }
}

void ZxVg93::execAccurateTypeII() {
    switch (CURR_STATE) {
    case 1:
        fBusy = BUSY;
        fDRQ = 0;
        fLOST = 0;
        fCRC_ERR = 0;
        fREC_NF = 0;
        fWFAULT = 0;
        fREC_TYP = 0;
        pCTRL->BSRR = EM_DRQ | EM_INTRQ;   // сброс
        Next_Byte = false;
        CURR_STATE++;
        break;

    case 2: // CPRDY не проверяем, он на спеке всегда готов и == HLD (Motor)
        if (!HLD) {
            dataTimerStart();
        } else {
            dataTimerContinue();
        }
        CURR_STATE++;
        break;

    case 3:
        if (CMD_R & CMDII_E) {
            delayStart(3000);
            CURR_STATE++;
        } else {
            CURR_STATE = 5;
        }
        break;

    case 4:
        if ((TIM6->CR1 & TIM_CR1_CEN) == 0) {
            CURR_STATE++;
        }
        break;

    case 5:
        if (pCTRL->IDR & EM_HLT) {
            CURR_STATE++;
        }
        break;

    case 6: // 4
        if ((CMD_R & CMD5) > 0 && fWRPT != 0) {
            CURR_STATE = 24;    // выход
        } else {
            if (!HLD) {
                dataTimerStart();
            } else {
                dataTimerContinue();
            }
            CURR_STATE++;
        }
        break;

    case 7: // 1
        if (IP_Counter > 5) {
            fREC_NF = REC_NF;
            CURR_STATE = 24;    // выход
        } else {
            CURR_STATE++;
        }
        break;

    case 8:
        if (Next_Byte) {
            Next_Byte = false;
            temp2 = Curr_Byte;
            SYNCPointer = disk->buffer() + MAX_TRACK_LEN + (Curr_Byte >> 3);
            if ((pCTRL->IDR & EM_SIDE) == 0) {
                temp2 += MAX_TRACK_LEN + MAX_BF_LEN;
                SYNCPointer += MAX_TRACK_LEN + MAX_BF_LEN;
            }
            DWPointer = (uint32_t*) (disk->buffer() + temp2);
            if (*DWPointer == IAM && (*SYNCPointer & 1 << (Curr_Byte & 7)) != 0) {
                if (*((uint8_t*) DWPointer + 4) == TRACK_R && ((*((uint8_t*) DWPointer + 5) == ((CMD_R & CMDII_s) != 0)) ||
                    ((CMD_R & CMDII_C) == 0)) &&
                    *((uint8_t*) DWPointer + 6) == SECT_R) {
                    switch (*((uint8_t*) DWPointer + 7) & 3) {
                    case 0: Sec_Length = 128; break;
                    case 1: Sec_Length = 256; break;
                    case 2: Sec_Length = 512; break;
                    default: Sec_Length = 1024;
                    }
                    CRCReset = true;
                    crc16(0xA1);
                    ADDRPointer = (uint8_t*) DWPointer + 1;
                    CURR_STATE++;
                } else {
                    CURR_STATE--;
                }
            }
        } else {
            CURR_STATE--;
        }
        break;

    case 9:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 8) {
                if (*ADDRPointer == (uint8_t) (CRC16_value >> 8)) {
                    CRCL_Saved = (uint8_t) (CRC16_value & 0x00FF);
                    fCRC_ERR = 0;
                } else {
                    fCRC_ERR = CRC_ERR;
                    CURR_STATE = 7;
                }
            } else if (CRC16_Byte_Counter == 9) {
                if (*ADDRPointer == CRCL_Saved) {
                    fCRC_ERR = 0;
                    CURR_STATE++;
                } else {
                    fCRC_ERR = CRC_ERR;
                    CURR_STATE = 7;
                }
            }
            crc16(*ADDRPointer++);
        }
        break;

    case 10:
        if (CMD_R & CMD5) {
            disk->setUnsavedCylinder(true);
            CURR_STATE = 15;    // запись сектора
        } else {
            CURR_STATE++;
        }
        break;

    case 11:    // 2
        if (Next_Byte) {
            Next_Byte = false;
            temp2 = Curr_Byte;
            SYNCPointer = disk->buffer() + MAX_TRACK_LEN + (Curr_Byte >> 3);
            if ((pCTRL->IDR & EM_SIDE) == 0) {
                temp2 += MAX_TRACK_LEN + MAX_BF_LEN;
                SYNCPointer += MAX_TRACK_LEN + MAX_BF_LEN;
            }
            DWPointer = (uint32_t*) (disk->buffer() + temp2);
            if ((*DWPointer & IDAM) == IDAM && (*SYNCPointer & 1 << (Curr_Byte & 7)) != 0) {
                if (*((uint8_t*) DWPointer + 3) == 0xF8) {
                    fREC_TYP = REC_TYP;
                } else {
                    fREC_TYP = 0;
                }
                CRCReset = true;
                crc16(0xA1);
                ADDRPointer = (uint8_t*) DWPointer + 1;
                CURR_STATE++;
            }
        } else if (IP_Counter > 5) {
            CURR_STATE = 7;
        }
        break;

    case 12:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 4) {
                CURR_STATE++;
            } else {
                crc16(*ADDRPointer++);
            }
        } else if (IP_Counter > 5) {
            CURR_STATE = 7;
        }
        break;

    case 13:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == Sec_Length + 4) {
                if (*ADDRPointer++ == (uint8_t) (CRC16_value >> 8) &&
                    *ADDRPointer == (uint8_t) (CRC16_value & 0x00FF)) {
                    fCRC_ERR = 0;
                    CURR_STATE++;
                } else {
                    fCRC_ERR = CRC_ERR;
                    CURR_STATE = 24; // выход
                }
            } else if (pCTRL->IDR & EM_DRQ) {
                fLOST = LOST;
            } else {
                DATA_R = *ADDRPointer;
                crc16(*ADDRPointer++);
                pCTRL->BSRR = EM_DRQ;  // установка
                fDRQ = DRQ;
            }
        } else if (IP_Counter > 5) {
            CURR_STATE = 7;
        }
        break;

    case 14:    // 5
        if (CMD_R & CMDII_m) {
            SECT_R++;
            CURR_STATE = 6;
        } else {
            CURR_STATE = 24;    // выход
        }
        break;

    case 15:    // 3
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 12) {
                // пропустим 2 байта
                pCTRL->BSRR = EM_DRQ;// Установка DRQ
                fDRQ = DRQ;
                CURR_STATE++;
            }
            crc16(*ADDRPointer++);
        }
        break;

    case 16:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 20) {
                // пропустим 8 байт
                if (pCTRL->IDR & EM_DRQ) {
                    fLOST = LOST;
                    CURR_STATE = 24; // выход
                } else {
                    CURR_STATE++;
                }
            }
            crc16(*ADDRPointer++);
        }
        break;

    case 17:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 32) {
                // конец зазора 0x4E
                *ADDRPointer = 0;
                CURR_STATE++;
            }
            crc16(*ADDRPointer++);
        }
        break;

    case 18:
        // чистка полей SYNCPointer где ???!!!
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 44) {
                // 12 нулей
                CRCReset = true;
                SYNCPointer = disk->buffer() + MAX_TRACK_LEN + ((ADDRPointer - disk->buffer()) >> 3);
                if ((pCTRL->IDR & EM_SIDE) == 0) {
                    SYNCPointer += MAX_TRACK_LEN + MAX_BF_LEN;
                }
                *SYNCPointer = 1 << ((uint32_t) (ADDRPointer - disk->buffer()) & 7);
                crc16(0xA1);
                *ADDRPointer++ = 0xA1;
                CURR_STATE++;
            } else {
                *ADDRPointer++ = 0;
                crc16(0);
            }
        }
        break;

    case 19:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 3) {
                if (CMD_R & CMDII_A0) {
                    *ADDRPointer++ = 0xF8;
                    crc16(0xF8);
                } else {
                    *ADDRPointer++ = 0xFB;
                    crc16(0xFB);
                }
                CURR_STATE++;
            } else {
                *ADDRPointer++ = 0xA1;
                crc16(0xA1);
            }
        }
        break;

    case 20:
        if (Next_Byte) {
            Next_Byte = false;
            pCTRL->BSRR = EM_DRQ;  // Установка DRQ
            fDRQ = DRQ;
            *ADDRPointer++ = DATA_R;
            crc16(DATA_R);
            CURR_STATE++;
        }
        break;

    case 21:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == Sec_Length + 4) {
                *ADDRPointer++ = (uint8_t) (CRC16_value >> 8);
                CRCL_Saved = (uint8_t) (CRC16_value & 0x00FF);
                crc16((uint8_t) (CRC16_value >> 8));
                CURR_STATE++;
            } else if (pCTRL->IDR & EM_DRQ) {
                fLOST = LOST;
                *ADDRPointer++ = 0;
                crc16(0);
            } else {
                pCTRL->BSRR = EM_DRQ;  // Установка DRQ
                fDRQ = DRQ;
                *ADDRPointer++ = DATA_R;
                crc16(DATA_R);
            }
        }
        break;

    case 22:
        if (Next_Byte) {
            Next_Byte = false;
            *ADDRPointer++ = CRCL_Saved;
            CURR_STATE++;
        }
        break;

    case 23:
        if (Next_Byte) {
            Next_Byte = false;
            *ADDRPointer++ = 0xFF;
            CURR_STATE = 14;
        }
        break;

    case 24:
        fBusy = 0;
        pCTRL->BSRR = EM_INTRQ;    // установка
        CURR_STATE = 0;
        CMD_TYPE = 0;
        break;

    default: break;
    }
}

void ZxVg93::execAccurateTypeIII() {
    switch (CURR_STATE) {
    case 1:
        fBusy = BUSY;
        fDRQ = 0;
        fLOST = 0;
        fCRC_ERR = 0;
        fREC_NF = 0;
        fWFAULT = 0;
        fREC_TYP = 0;
        pCTRL->BSRR = EM_DRQ | EM_INTRQ;   // сброс
        Next_Byte = false;
        CURR_STATE++;
        break;

    case 2:
        if (!HLD) {
            dataTimerStart();
        } else {
            dataTimerContinue();
        }
        CURR_STATE++;
        break;

    case 3:
        if (CMD_R & CMDII_E) {
            delayStart(3000);
            CURR_STATE++;
        } else {
            CURR_STATE = 5;
        }
        break;

    case 4:
        if ((TIM6->CR1 & TIM_CR1_CEN) == 0) {
            CURR_STATE++;
        }
        break;

    case 5:
        if (pCTRL->IDR & EM_HLT) {
            CURR_STATE++;
        }
        break;

    case 6:
        if ((CMD_R & CMD5) == 0) {
            CURR_STATE++;   // чтение адреса
        } else if ((CMD_R & CMD4) == 0) {
            CURR_STATE = 10;    // чтение дорожки
        } else {
            CURR_STATE = 12;    // запись дорожки
        }
        break;

    case 7:
        if (IP_Counter > 5) {
            fREC_NF = REC_NF;
            CURR_STATE = 10;    // выход
        } else {
            CURR_STATE++;
        }
        break;

    case 8:
        if (Next_Byte) {
            Next_Byte = false;
            temp2 = Curr_Byte;
            SYNCPointer = disk->buffer() + MAX_TRACK_LEN + (Curr_Byte >> 3);
            if ((pCTRL->IDR & EM_SIDE) == 0) {
                temp2 += MAX_TRACK_LEN + MAX_BF_LEN;
                SYNCPointer += MAX_TRACK_LEN + MAX_BF_LEN;
            }
            DWPointer = (uint32_t*) (disk->buffer() + temp2);
            if (*DWPointer == IAM && (*SYNCPointer & 1 << (Curr_Byte & 7)) != 0) {
                CRCReset = true;
                crc16(0xA1);
                ADDRPointer = (uint8_t*) DWPointer + 1;
                CURR_STATE++;
            } else {
                CURR_STATE--;
            }
        } else {
            CURR_STATE--;
        }
        break;

    case 9:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 10) {
                CURR_STATE = 18;    // выход
            } else if ((CRC16_Byte_Counter > 3) && (CRC16_Byte_Counter < 10)) {
                if (CRC16_Byte_Counter == 8) {
                    if (*ADDRPointer == (uint8_t) (CRC16_value >> 8)
                        && *(ADDRPointer + 1) == (uint8_t) (CRC16_value & 0x00FF)) {
                        fCRC_ERR = 0;
                    } else {
                        fCRC_ERR = CRC_ERR;
                    }
                }
                if (CRC16_Byte_Counter == 4) {
                    SECT_R = *ADDRPointer;
                }
                DATA_R = *ADDRPointer;
                pCTRL->BSRR = EM_DRQ;  // установка
                fDRQ = DRQ;
            }
            crc16(*ADDRPointer++);
        } else if (IP_Counter > 5) {
            CURR_STATE = 7;
        }
        break;

    case 10:
        // чтение дорожки
        if (Next_Byte) {
            Next_Byte = false;
            if (Curr_Byte == INDEX_START) {
                ADDRPointer = disk->buffer();
                if ((pCTRL->IDR & EM_SIDE) == 0)
                    ADDRPointer += MAX_TRACK_LEN + MAX_BF_LEN;
                DATA_R = *ADDRPointer++;
                pCTRL->BSRR = EM_DRQ;  // установка
                fDRQ = DRQ;
                CURR_STATE++;
            }
        }
        break;

    case 11:
        if (Next_Byte) {
            Next_Byte = false;
            if (pCTRL->IDR & EM_DRQ) {
                fLOST = LOST;
            }
            if (Curr_Byte != INDEX_START) {
                ADDRPointer = disk->buffer() + Curr_Byte;
                if ((pCTRL->IDR & EM_SIDE) == 0) {
                    ADDRPointer += MAX_TRACK_LEN + MAX_BF_LEN;
                }
                DATA_R = *ADDRPointer++;
                pCTRL->BSRR = EM_DRQ;  // установка
                fDRQ = DRQ;
            } else {
                CURR_STATE = 18;    // выход
            }
        }
        break;

    case 12:
        // запись дорожки
        if (fWRPT != 0) {
            CURR_STATE = 18;    // выход
        } else {
            //pCTRL->BSRRL = EM_TST;
            if ((pCTRL->IDR & EM_SIDE) == 0) {
                DWPointer = (uint32_t*) (disk->buffer() + MAX_TRACK_LEN * 2 + MAX_BF_LEN);
            } else {
                DWPointer = (uint32_t*) (disk->buffer() + MAX_TRACK_LEN);
            }
            for (j = 0; j < MAX_BF_LEN / 4; j++) {
                *DWPointer++ = 0;   // 21 мкс
            }
            //pCTRL->BSRRH = EM_TST;
            pCTRL->BSRR = EM_DRQ;  // установка
            fDRQ = DRQ;
            CRCReset = true;
            crc16(0);
            PREV_DATA = 0;
            CURR_STATE++;
        }
        break;

    case 13:
        if (Next_Byte) {
            Next_Byte = false;
            if (CRC16_Byte_Counter == 4) {
                if (pCTRL->IDR & EM_DRQ) {
                    fLOST = LOST;
                    CURR_STATE = 18;    // выход
                } else {
                    CURR_STATE++;
                    disk->setUnsavedCylinder(true);
                }
            } else {
                crc16(0);
            }
        }
        break;

    case 14:
        if (Next_Byte) {
            Next_Byte = false;
            if (Curr_Byte == INDEX_START) {
                ADDRPointer = disk->buffer();
                SYNCPointer = disk->buffer() + MAX_TRACK_LEN;
                if ((pCTRL->IDR & EM_SIDE) == 0) {
                    ADDRPointer += MAX_TRACK_LEN + MAX_BF_LEN;
                    SYNCPointer += MAX_TRACK_LEN + MAX_BF_LEN;
                }
                switch (DATA_R) {
                case 0xF5:
                    *ADDRPointer = 0xA1;
                    if (PREV_DATA != 0xA1) {
                        *SYNCPointer = 1;   // INDEX_START = 0 и не изменяется
                        CRCReset = true;
                    }
                    crc16(0xA1);
                    PREV_DATA = 0xA1;
                    CURR_STATE += 2;
                    pCTRL->BSRR = EM_DRQ;  // установка
                    fDRQ = DRQ;
                    break;

                case 0xF6:
                    *ADDRPointer = 0xC2;
                    if (PREV_DATA != 0xC2) {
                        *SYNCPointer = 1;
                    }
                    PREV_DATA = 0xC2;
                    CURR_STATE += 2;
                    pCTRL->BSRR = EM_DRQ;  // установка
                    fDRQ = DRQ;
                    break;

                case 0xF7:
                    CRCL_Saved = (uint8_t) (CRC16_value & 0x00FF);
                    *ADDRPointer++ = (uint8_t) (CRC16_value >> 8);
                    PREV_DATA = (uint8_t) (CRC16_value >> 8);
                    CURR_STATE++;
                    break;

                default:
                    *ADDRPointer = DATA_R;
                    crc16(DATA_R);
                    PREV_DATA = DATA_R;
                    CURR_STATE += 2;
                    pCTRL->BSRR = EM_DRQ;// установка
                    fDRQ = DRQ;
                }
            }
        }
        break;

    case 15:
        if (Next_Byte) {
            Next_Byte = false;
            PREV_DATA = CRCL_Saved;
            *ADDRPointer = CRCL_Saved;
            CURR_STATE++;
            pCTRL->BSRR = EM_DRQ;  // установка
            fDRQ = DRQ;
        }
        break;

    case 16:
        if (Next_Byte) {
            Next_Byte = false;
            if (Curr_Byte != INDEX_START) {
                ADDRPointer = disk->buffer() + Curr_Byte;
                SYNCPointer = disk->buffer() + MAX_TRACK_LEN + (Curr_Byte >> 3);
                if ((pCTRL->IDR & EM_SIDE) == 0) {
                    ADDRPointer += MAX_TRACK_LEN + MAX_BF_LEN;
                    SYNCPointer += MAX_TRACK_LEN + MAX_BF_LEN;
                }
                if (pCTRL->IDR & EM_DRQ) {
                    fLOST = LOST;
                    *ADDRPointer = 0;
                    crc16(0);
                    PREV_DATA = 0;
                } else {
                    switch (DATA_R) {
                    case 0xF5:
                        *ADDRPointer = 0xA1;
                        if (PREV_DATA != 0xA1) {
                            *SYNCPointer = 1 << (Curr_Byte & 7);
                            CRCReset = true;
                        }
                        crc16(0xA1);
                        PREV_DATA = 0xA1;
                        pCTRL->BSRR = EM_DRQ;  // установка
                        fDRQ = DRQ;
                        break;

                    case 0xF6:
                        *ADDRPointer = 0xC2;
                        if (PREV_DATA != 0xC2) {
                            *SYNCPointer = 1 << (Curr_Byte & 7);
                        }
                        PREV_DATA = 0xC2;
                        pCTRL->BSRR = EM_DRQ;  // установка
                        fDRQ = DRQ;
                        break;

                    case 0xF7:
                        CRCL_Saved = (uint8_t) (CRC16_value & 0x00FF);
                        *ADDRPointer++ = (uint8_t) (CRC16_value >> 8);
                        PREV_DATA = (uint8_t) (CRC16_value >> 8);
                        CURR_STATE++;
                        break;

                    default:
                        *ADDRPointer = DATA_R;
                        crc16(DATA_R);
                        PREV_DATA = DATA_R;
                        pCTRL->BSRR = EM_DRQ;// установка
                        fDRQ = DRQ;
                    }
                }
            } else {
                CURR_STATE = 18;    // выход
            }
        }
        break;

    case 17:
        if (Next_Byte) {
            Next_Byte = false;
            PREV_DATA = CRCL_Saved;
            *ADDRPointer = CRCL_Saved;
            CURR_STATE--;
            pCTRL->BSRR = EM_DRQ;  // установка
            fDRQ = DRQ;
        }
        break;

    case 18:
        // выход
        fBusy = 0;
        pCTRL->BSRR = EM_INTRQ;// установка
        CURR_STATE = 0;
        CMD_TYPE = 0;
        break;

    default:
        break;
    }
}

void ZxVg93::execAccurateTypeIV() {
    switch (CURR_STATE) {
    case 1:
        fBusy = 0;
        CURR_STATE++;
        break;

    case 2:
        if ((CMD_R & CMDIV_I03) == 0) {
            // немедленно без INTRQ
            CURR_STATE = 4;
        } else if (CMD_R & CMDIV_I3) {
            // немедленно с INTRQ
            CURR_STATE++;
        } else if (CMD_R & CMDIV_I2) {
            // индексный импульс
            if (disk->oldIndex() == 0 && fIndex != 0) {
                CURR_STATE++;
            } else if (CMD_R & CMDIV_I1) {
                // готов -> не готов
                if (false) {
                    // на спеке такой переход не произойдет
                    CURR_STATE++;
                } else if (CMD_R & CMDIV_I0) {
                    // не готов -> готов
                    if (false) { // на спеке такой переход не произойдет
                        CURR_STATE++;
                    }
                }
            }
        }
        disk->setOldIndex(fIndex);
        //          OldCprdy = ;    // на будущее
        break;

    case 3:
        dataTimerStop();
        CURR_STATE++;
        pCTRL->BSRR = EM_INTRQ;    // установка
        break;

    case 4:
        CURR_STATE = 0;
        CMD_TYPE = 0;
        break;

    default:
        break;
    }
}

void ZxVg93::dataTimerConfig() {
    // TIM7 clock enable
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    TIM7->ARR = 127;    // время отсчета 32 мкс ( 60 МГц / 128 / 15 )
    TIM7->PSC = 14;    // предделитель 15
    TIM7->CR1 |= TIM_CR1_ARPE;    // ARR Preload+
// NVIC_IRQChannelPreemptionPriority = 15
    NVIC->IP[TIM7_IRQn] = 0x0F;
    // NVIC_IRQChannelCmd = ENABLE
    NVIC->ISER[TIM7_IRQn >> 5] = 1 << (TIM7_IRQn & 0x1F);
    //
    dataTimerStop();
}

void ZxVg93::dataTimerStart() {
    ADDRPointer = NULL;
    IP_Counter = 0;
    Curr_Byte = 0;
    TIM7->EGR = TIM_EGR_UG;     // сброс счетчика
    TIM7->DIER = TIM_DIER_UIE;     // разрешаем прерывания
    TIM7->CR1 |= TIM_CR1_CEN;     // запускаем счет
    HLD = true;     // мотор
}

void ZxVg93::dataTimerContinue() {
    //      ADDRPointer = NULL;
    IP_Counter = 0;
}

void ZxVg93::dataTimerStop() {
    TIM7->DIER = 0;
    TIM7->CR1 &= ~TIM_CR1_CEN;  // Останавливаем счет
    HLD = false;
    if (disk->unsavedCylinder()) {
        disk->saveCylinder();
    }
    //  if ( Unsaved_CylinderB )    Save_CylinderB();
}

void ZxVg93::delayConfig() {
    // TIM6 clock enable
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->ARR = 65535;  // период импульсов
    TIM6->PSC = 599;  // предделитель 600 -> 100 кГц
    TIM6->CR1 |= TIM_CR1_ARPE | TIM_CR1_OPM;  // ARR Preload+, One Pulse Mode
}

void ZxVg93::delayStart(uint16_t TIM_ARR) {
    TIM6->CR1 &= ~TIM_CR1_CEN;  // Остановим счет, если вдруг он идет
    TIM6->CNT = 0;  // сброс счетчика
    TIM6->ARR = TIM_ARR;  // кол-во мс умножаем на сто = ARR
    TIM6->CR1 |= TIM_CR1_CEN;  // Запускаем счет
}

void fault_err(FRESULT rc) {
    const char* str = "OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
        "INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
        "INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
        "LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0";
    int i;

    for (i = 0; i != rc && *str; ++i) {
        while (*str++)
            ;
    }
    /*    xprintf((const char*) str);
     #ifdef ERROR_RESET
     Delay(75);
     NVIC_SystemReset();
     #else*/
    while (1)
        ;
    //#endif
}

/*
 void GPIO_PreConfig(void) {
 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
 // GPIOA pins Init
 GPIOA->MODER = 0xA8280000;
 GPIOA->OSPEEDR = 0xFFFFFFFF;
 GPIOA->PUPDR = 0x00000000;
 GPIOA->OTYPER = 0x0000;
 GPIOA->AFR[0] = 0x00000000;
 GPIOA->AFR[1] = 0x00000770;
 // GPIOB pins Init
 GPIOB->MODER = 0x00005280;
 GPIOB->OSPEEDR = 0xFFFFFFFF;
 GPIOB->PUPDR = 0x00000000;
 GPIOB->OTYPER = 0x00C0; //DRQ INTRQ
 GPIOB->AFR[0] = 0x00000000;
 GPIOB->AFR[1] = 0x00000000;
 // GPIOC pins Init
 GPIOC->MODER = 0x02AA0000;
 GPIOC->OSPEEDR = 0xFFFFFFFF;
 GPIOC->PUPDR = 0x00000000;
 GPIOC->OTYPER = 0x0000;
 GPIOC->AFR[0] = 0x00000000;
 GPIOC->AFR[1] = 0x000CCCCC;
 // GPIOD pins Init
 GPIOD->MODER = 0x00000020;
 GPIOD->OSPEEDR = 0xFFFFFFFF;
 GPIOD->PUPDR = 0x00000000;
 GPIOD->OTYPER = 0x0000;
 GPIOD->AFR[0] = 0x00000C00;
 GPIOD->AFR[1] = 0x00000000;
 }

 void EXTI_Config(void) {
 // Enable SYSCFG to set up EXTIs
 RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
 // NVIC_IRQChannelPreemptionPriority = 15
 NVIC->IP[EXTI2_IRQn] = 0x0F;
 // NVIC_IRQChannelCmd = ENABLE
 NVIC->ISER[EXTI2_IRQn >> 5] = 1 << (EXTI2_IRQn & 0x1F);
 // NVIC_IRQChannelPreemptionPriority = 0
 NVIC->IP[EXTI9_5_IRQn] = 0;
 // NVIC_IRQChannelCmd = ENABLE
 NVIC->ISER[EXTI9_5_IRQn >> 5] = 1 << (EXTI9_5_IRQn & 0x1F);
 // NVIC_IRQChannelPreemptionPriority = 0
 NVIC->IP[EXTI15_10_IRQn] = 0;
 // NVIC_IRQChannelCmd = ENABLE
 NVIC->ISER[EXTI15_10_IRQn >> 5] = 1 << (EXTI15_10_IRQn & 0x1F);
 // Connect EXTI Line2 to PB2 pin
 SYSCFG->EXTICR[EXTI_PinSource2 >> 2] |= (uint32_t) SYSCFG_EXTICR1_EXTI2_PB;
 // Connect EXTI Line5 to PB5 pin
 SYSCFG->EXTICR[EXTI_PinSource5 >> 2] |= (uint32_t) SYSCFG_EXTICR2_EXTI5_PB;
 // Connect EXTI Line10 to PB10 pin
 SYSCFG->EXTICR[EXTI_PinSource10 >> 2] |= (uint32_t) SYSCFG_EXTICR3_EXTI10_PB;
 // Enable EXTI Lines 2,5 & 10
 EXTI->IMR |= EXTI_Line2 | EXTI_Line5 | EXTI_Line10;
 // Select Falling trigger for EXTI Lines 2
 EXTI->FTSR |= EXTI_Line2;
 // Select Rising trigger for EXTI Lines 5 & 10
 EXTI->RTSR |= EXTI_Line5 | EXTI_Line10;
 }
 */
