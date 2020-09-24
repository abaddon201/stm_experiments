/*
 * zx_trd_feeder.h
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_ZX_TRD_FEEDER_H_
#define INC_ZX_TRD_FEEDER_H_

#include <stdio.h>

#include "../../_zx/Inc/zx_disk_feeder.h"

class ZxTrdFeeder: public ZxDiskFeeder {
public:
    bool parseImageFile(const char *fileName) override;

    void loadCylinder(uint32_t Cyl_Number) override;
    void saveCylinder() override;

    bool init() override;

};
#endif /* INC_ZX_TRD_FEEDER_H_ */
