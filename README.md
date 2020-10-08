# stm32-polyphonic-tunes

#### *Esta API foi desenvolvida como trabalho da disciplina de Programação de Sistemas Embarcados da UFMG*
#### *- Prof. Ricardo de Oliveira Duarte – Departamento de Engenharia Eletrônica*

# Important!

This library is importing the HAL library from stm32H7xx, you should change it to match your stm model. Access the library source files and change it. 

## Hardware connections

                         
                       +10µF 
 Output Pin ---[ 1k ]--+---||--->> Audio out
                     
                     |
                     
                   === 10nF
                   
                     |
                     
                    GND

## How to write song: 

You may be wondering how one can write songs for the stm32 without any knowledge of Music! Its actually quite interesting and fun. First you need to know the value of the notes in Music. 

![alt text](https://lh3.googleusercontent.com/proxy/14r8O6HwR5brg3J6wkm-n_To6zeHLZB4NYLGDut-vAvWiy9ThKSZgXc-cgZFelgwnJNpaGx0N3o3rPfhI0D5kC5FCgTrcOc8eOZOWPJ59f1bdCLnIw-_emWDQtB4gDhNRk1SF9ryGhKeUAoqk-2c6U_PdYc)

The API only can play 4 voices together.

### The stm32 Code:

#### Struct **music**

Formed by 5 parameters: octave, bpm, control, voices and nota ref. 

##### The octave, the bpm and the ref note are fixed parameters of the music.

**octave**: octave reference. Usually the reference is the middle C (C4). 
*By default, notes are associated with the first octave: c1.*
**Formula: note = note + 12*(octave-1)**

**bpm**: beats per minute - the tempo of the song. 
**ref note**: the note ()

**voice**: music *string.*
- example: *music.voices[0] = "4g1,8p,8d1,4g1,8p,8d1"*

The control of the voice string is done through 5 parameters of the struct song_ctrl.

**note**: contains the FTW (Frequency Tuning Word) table index corresponding to the note.
**duration**: contains the time the note will be played based on the to be presented equation.
**position**: the index of the string related to note and tempo. That's the engine reference on the current position of the song.

**pause_ticks**: number of clock pulses which the note is not played.   }
**remaining_ticks**: number of clock pulses which the note is played.   } The sum of the two parameters is the total note time

You can change the NR_PERC definition inside song_engine.h in order to redistribute the ration between playing and pausing within a note. The higher the value, the more the notes sound separated.

**Voice and control has voice reference indicators. It is extremely important to use the same indices for the same voices.**
- example to reference the same voice:
  - music.voice[*0*]; 
  - music.control.note[*0*]; 
  - music.control.note[*0*];

#### Note request:
##### The song's notes are made after the second semicolon.

##### Time:
- The duration of each note is made before the letter
  - ex: 4f is a quarter note
- Put a period . for dotted notes
  - ex: 2.f is a dotted half note
- The duration can be determined by this formula.
 **Formula: (ref_note)/(note value)= duration. So an eighth note would be 4/8 = 1/2.**

##### Note:

- There are only 7 possible letters. a,b,c,d,e,f,g
- A rest uses the letter p
- To name a letter a flat put an underscore _ right after the letter
  - ex: b_
- To name a letter a sharp put a hashtag # right after the letter
  - ex: c#
- To raise a note up an octave, put the number of octaves to be raised after the Sharp/Flats/Letter
  - ex: b1
  - ex2: c#1
- To drop a note down an Octave, put a minus - sign and then the number of octaves to be dropped after the Sharp/Flats/Letter
  - ex: b-1
  - ex: c#-2

#### Creating a Song:


- music song;

- song.octave = 4. // reference in middle c (c4)

- song.bpm = 100; 

- song.nota_ref = 4 // quarter_note

- song.voice = "8c,8d,8e,8f,8g,8a,8b,8c1"; // eighth note c-scale


##### You must not :

- Use spaces
- Use diferents caracters 

## How to read a note

To read the note you just have to know the control variables, and note_uptade gives it. Before using the note update, it is important to initialize the position field with null values.

**uint8_t note_update(music *song, uint8_t instrument)**
instrument represent the voice of the score. Usually in classical music, they have several instruments in which each represents a voice.

The function will uptade duration, position, note and will return 1 when the string have more notes to read. If not, return 0. The function return it to other the programmer know when the song is over.

example of an internal use:

uint8_t violine_I; 

note_update(&song, violine_I);

- song.control.note[violine_I] will be the number that represent c4 in the FTW table. 

- song.contol.duration[violine_I] will be the time -> 4(nota_ref)/8(time);



## How to produce sound

The library polyphonic_tunes make it! 

First you have to understand how the code produce play diferent notes at the same time.
Basicly it sums diferrent sin waves at the same time.
Read the article: https://www.gamry.com/application-notes/EIS/waveform-generation-and-frequency-resolution/

### Note:
- **Sine discretization**: take one period sine wave - 2pi - and divide in 2^10 bits to get 1024 samples.
*Note that is the SinTable in the polyphonic_tunes_tables.h. There's also a triangle table if you'd like that

- **FTW (Frequency Tuning Word)**: parameter that the code uses to synthesize the frequency note.
*Note that in the PITCHS table has the notes from c-2 to c8. Start in c-2 and goes one tone up until c8, so PITCHS[0] represent the FTW of c in octave -2.

- **Get better**: use a RC filter to get cleanner sound! It's explained in the beginning how to do so.

### How to use song_egine:

This is the library that you will use, it cointains the functions to control music.

# NOTE
The first thing you should do is setup a timer from your stm to 20kHz. You should also enable global interrupts from this timer. The formula to know the timer frequency based on its parameters is: Fout = Fbus/((Prescaler+1)\*(Period+1)). Where Fbus is the frequency of the bus the timer is connected. You should choose the Period and Prescaler values so that Fout equal 20kHz. You also must implement this timer's overflow interrupt, and call the library's function song_scheduler, passing the interrupt source timer as the parameter. **IT must be 20khz, otherwise you'll get distorted sound**.
If you want to use the stm's PWM as output, you should also set a spare timer and configure one of its channel as PWM, preferably configured as fast PWM. 
You can find an example inside the example folder.

I) You have to initialize the timer interruption (initialize_song_engine and song_scheduler), to make use the library.

- **ctrl_tim**: the 20kHz timer that you will use.

- **timer_freq**: timer bus frequency.


#### Timer and PWM configuration:


- **Your timer input frequency must has 20kHz**

- **Put PWM in the max frequency possible and put the resolution to 2^11 bits** 

  - void initialize_song_engine(double timer_freq, TIM_HandleTypeDef* ctrl_tim);
  
- **You MUST call this function inside your 20kHz timer callback, and pass the associated timer instance with it. You must implement the callback if you haven't done so.** 
  - void song_scheduler(TIM_HandleTypeDef* htim);



II) After that, you can treat output with pwm or use a custom treatment output.

  - void set_pwm_output(TIM_HandleTypeDef* output_tim, uint8_t out_channel);
  - void set_custom_output_handler(void (*output_handler)(uint32_t));



III) Set the music struct parameters and load the song. 
  - music musica;
  - musica.bpm = 60;
  - musica.nota_ref = QUARTER_NOTE;
  - musica.oitava = 4;
  - musica.voice[0] = <your_song_string>;.
  - ...
  - void load_song(music musica);



IV) Just play and have fun. Use the play_song() to start playing. Use stop_song() to move the engine reference to the top, and play_song() again to replay it.

You can use other functions to set/get the status of the song.

- void clear_song();

- void play_song();

- void pause_song();

- void stop_song();

- song_status get_song_status();



### How to use polyphonic_tune:

This library will join the 4 voices.

I) Set the pwm and timer necessaries, like the song_engine library. - You can use both library independently

II) You can customize your sound with diferents sets like sin wave(setwave).
But you have to set the function in **bold**.

- void setWave(uint8_t voice, uint8_t wave);

- void setPitch(uint8_t voice, uint8_t MIDInote);

- void setEnvelope(uint8_t voice, uint8_t env);

- void setLength(uint8_t voice, uint8_t length);

- void setMod(uint8_t voice, uint16_t mod);

- void mTrigger(uint8_t voice, uint8_t MIDInote);

- void setFrequency(uint8_t voice, float f);

- void setTime(uint8_t voice, float t);

- void trigger(uint8_t voice);


III) Set the voices

- void setupVoice(uint8_t voice, uint8_t wave, uint8_t pitch, uint8_t env, uint8_t length, uint16_t mod);

IV) Join the voices with synthesis. You must call this function inside your time callback, as you did in song_engine

- void audio_synthesis();

You can modifed the status with:
- void pause(uint8_t voice);
- void synth_suspend();
- void synth_resume();
