/*
 * zx_screen.h
 *
 *  Created on: Sep 6, 2020
 *      Author: user
 */

#ifndef INC_ZX_SCREEN_H_
#define INC_ZX_SCREEN_H_

#include <stdio.h>

class ZxScreen {
    uint16_t brightColors[8];
    uint16_t darkColors[8];

public:
    ZxScreen();
};


#endif /* INC_ZX_SCREEN_H_ */
