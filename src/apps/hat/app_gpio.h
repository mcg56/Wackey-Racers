/** @file   app_gpio.h
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to general purpose pio 
            (leds, buttons etc)
*/

#ifndef APP_GPIO_H
#define APP_GPIO_H
#define NUM_LEDS 23

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define LOW_BAT_LED_RATE  2 //Hz

/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
void pio_configuration(void);
void flash_led(int led_pio, int num_flash);
void green_strip(void);
void red_strip(void);
void blue_strip(void);
void empty_strip(void);

#endif //APP_GPIO_H