/*
 * zx_disk_feeder.h
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_ZX_DISK_FEEDER_H_
#define INC_ZX_DISK_FEEDER_H_

#include <stdio.h>
#include "fatfs.h"

#define MAX_TRACK_LEN 6400              // декодированные байты
#define MAX_BF_LEN MAX_TRACK_LEN/8      // битовое поле

#define CYL_NUM_MAX 84

class ZxDiskFeeder {
protected:
    uint8_t Max_CylinderA;
    bool Unsaved_CylinderA;
    uint32_t Loaded_CylinderA;
    uint8_t OldIndexA;
    FIL file; /* File object */
    bool d_isReadonly;

    uint8_t buffA[(MAX_TRACK_LEN + MAX_BF_LEN) * 2];
    uint32_t track_lengsA[(CYL_NUM_MAX + 1) * 2];
    uint32_t track_offsetsA[(CYL_NUM_MAX + 1) * 2];

public:
    inline uint8_t maxCylinder() {
        return Max_CylinderA;
    }
    inline bool unsavedCylinder() {
        return Unsaved_CylinderA;
    }
    inline void setUnsavedCylinder(bool val) {
        Unsaved_CylinderA = val;
    }
    inline uint32_t loadedCylinder() {
        return Loaded_CylinderA;
    }
    inline uint8_t oldIndex() {
        return OldIndexA;
    }
    inline void setOldIndex(uint8_t val) {
        OldIndexA = val;
    }
    inline bool isReadonly() {
        return d_isReadonly;
    }

    inline uint8_t* buffer() {
        return buffA;
    }

    virtual bool parseImageFile(const char *fileName) = 0;

    virtual void loadCylinder(uint32_t Cyl_Number) = 0;
    virtual void saveCylinder() = 0;

    virtual bool init() = 0;

};

#endif /* INC_ZX_DISK_FEEDER_H_ */
