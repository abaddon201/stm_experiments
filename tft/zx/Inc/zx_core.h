/*
 * zx_core.h
 *
 *  Created on: 23 сент. 2020 г.
 *      Author: Abby
 */

#ifndef INC_ZX_CORE_H_
#define INC_ZX_CORE_H_

// ROM control register
#define ROM_CTRL GPIOB
#define ROM_A14 ((uint32_t) 1<<0 )
#define ROM_A15 ((uint32_t) 1<<1 )
#define ROM_A16 ((uint32_t) 1<<2 )
#define ROM_WE ((uint32_t) 1<<3 )
#define ROM_CE ((uint32_t) 1<<4 )
#define ROM_OE ((uint32_t) 1<<5 )

// 16-bits addr line
#define REG_A GPIOF
// 8-bit data line
#define REG_D GPIOG


class ZxCore {
public:
    void startCPUClock();
    void startAYClock();
    void startIntClock();
};

#endif /* INC_ZX_CORE_H_ */
