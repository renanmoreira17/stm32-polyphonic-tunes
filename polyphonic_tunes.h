/*
 * polyphonic-tunes.h
 *
 *  Created on: Oct 5, 2020
 *      Author: Renan Moreira, Rodolfo Lessa
 *     Version: 1.0
 *     License: GPLv3.0
 */

#ifndef INC_POLYPHONIC_TUNES_H_
#define INC_POLYPHONIC_TUNES_H_

#include "stm32h7xx_hal.h"
#include "stdint.h"

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


void audio_synthesis();
void setup_synth_engine(double timer_frequency, TIM_HandleTypeDef* ctrl_tim);
void setup_synth_custom_output_handler(void (*output_handler)(uint32_t));
void setup_synth_pwm_output_handler(TIM_HandleTypeDef* output_tim, uint8_t out_channel);
void setupVoice(uint8_t voice, uint8_t wave, uint8_t pitch, uint8_t env, uint8_t length, uint16_t mod);
void setWave(uint8_t voice, uint8_t wave);
void setPitch(uint8_t voice, uint8_t MIDInote);
void setEnvelope(uint8_t voice, uint8_t env);
void setLength(uint8_t voice, uint8_t length);
void setMod(uint8_t voice, uint16_t mod);
void mTrigger(uint8_t voice, uint8_t MIDInote);
void setFrequency(uint8_t voice, float f);
void setTime(uint8_t voice, float t);
void trigger(uint8_t voice);

void pause(uint8_t voice);

void synth_suspend();
void synth_resume();


#endif /* INC_POLYPHONIC_TUNES_H_ */
