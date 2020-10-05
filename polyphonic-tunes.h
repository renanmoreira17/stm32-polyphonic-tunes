/*
 * polyphonic-tunes.h
 *
 *  Created on: Oct 5, 2020
 *      Author: renanmoreira
 */

#ifndef INC_POLYPHONIC_TUNES_H_
#define INC_POLYPHONIC_TUNES_H_

#include "stm32h7xx_hal.h"

#define DIFF 1
#define CHA 2
#define CHB 3

#define SINE     0
#define TRIANGLE 1
#define SQUARE   2
#define SAW      3
#define RAMP     4
#define NOISE    5

#define ENVELOPE0 0
#define ENVELOPE1 1
#define ENVELOPE2 2
#define ENVELOPE3 3

#define FS 20000.0                                              //-Sample rate (NOTE: must match tables.h)





#endif /* INC_POLYPHONIC_TUNES_H_ */
