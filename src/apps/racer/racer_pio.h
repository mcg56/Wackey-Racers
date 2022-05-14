/** @file   racer_pio.h
    @author E.Twomey, M.Gardyne
    @date   4/05/2022
    @brief  Functions and structs relating to general purpose pio 
            (leds, buttons etc)
*/

#ifndef RACER_PIO_H
#define RACER_PIO_H

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define LOW_BAT_LED_RATE  2 //Hz
#define NUM_LEDS 28 // Number of LEDs in tape

/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
void init_pio(void);
void flash_led(int led_pio, int num_flash);
void green_strip(void);
void red_strip(void);
void blue_strip(void);

#endif //RACER_PIO_H