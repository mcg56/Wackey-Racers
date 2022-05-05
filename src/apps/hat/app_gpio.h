/** @file   app_gpio.h
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to general purpose pio 
            (leds, buttons etc)
*/

#ifndef APP_GPIO_H
#define APP_GPIO_H

/******************************************************************************
* CONSTANTS
******************************************************************************/


/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
void pio_configuration(void);
void flash_led(int led_pio, int num_flash);

#endif //APP_GPIO_H