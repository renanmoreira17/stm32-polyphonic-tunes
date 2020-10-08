/*
 * song_engine.h
 *
 *  Created on: Oct 7, 2020
 *      Author: renanmoreira
 */

#ifndef POLYPHONIC_TUNES_LIB_SONG_ENGINE_H_
#define POLYPHONIC_TUNES_LIB_SONG_ENGINE_H_

#include "stdint.h"
#include "stm32h7xx_hal.h"

typedef enum {
	WHOLE_NOTE   		= 1,
	HALF_NOTE	 		= 2,
	QUARTER_NOTE 		= 4,
	EIGHTH_NOTE	 		= 8,
	SIXTEENTH_NOTE 		= 16,
	THIRTY_SECOND_NOTE 	= 32,
	SIXTY_FOURTH_NOTE 	= 64
} note_value;


typedef enum {
	STOPPED,
	PLAYING,
	PAUSED
} song_status;

typedef struct {
	volatile uint8_t note[4];
	volatile uint32_t remaining_ticks[4];
	volatile uint32_t pause_ticks[4];
	volatile double duration[4];
	volatile uint16_t posicao[4];
} song_ctrl;

typedef struct {
	note_value nota_ref;
	uint16_t bpm;
	uint8_t oitava;

	const char* voices[4];

	volatile song_ctrl control;
} music;

#define NR_PERC 95.0


void initialize_song_engine(double timer_freq, TIM_HandleTypeDef* ctrl_tim);
void load_song(music musica);
void clear_song(music musica);
void set_pwm_output(TIM_HandleTypeDef* output_tim, uint8_t out_channel);
void set_custom_output_handler(void (*output_handler)(uint32_t));
void play_song();
void pause_song();
void stop_song();
void song_scheduler(TIM_HandleTypeDef* htim);
song_status get_song_status();



#endif /* POLYPHONIC_TUNES_LIB_SONG_ENGINE_H_ */
