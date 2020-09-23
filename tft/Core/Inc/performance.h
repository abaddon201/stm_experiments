/*
 * performance.h
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_PERFORMANCE_H_
#define INC_PERFORMANCE_H_

#include <stdio.h>

#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)

class Performance {
    uint32_t start;
    uint32_t stop;
    uint32_t delta;

    static bool isInited;

    static void init() {
        if (ARM_CM_DWT_CTRL != 0) {        // See if DWT is available
            ARM_CM_DEMCR |= 1 << 24;  // Set bit 24
            ARM_CM_DWT_CYCCNT = 0;
            ARM_CM_DWT_CTRL |= 1 << 0;   // Set bit 0
        }
        isInited = true;
    }

public:
    Performance() {
        if (!isInited) {
            init();
        }
        start = ARM_CM_DWT_CYCCNT;
    }

    ~Performance() {
        stop = ARM_CM_DWT_CYCCNT;
        delta = stop - start;
    }
};

#endif /* INC_PERFORMANCE_H_ */
