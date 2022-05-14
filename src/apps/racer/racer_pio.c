/** @file   racer_pio.c
    @author E. Twomey, M. Gardyne 
    @date   14/05/2022
    @brief  Functions and structs relating to general purpose pio 
            (leds, buttons etc)
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "target.h"
#include "pio.h"
#include "racer_pio.h"
#include "delay.h"
#include "ledbuffer.h"
#include "ledtape.h"




/******************************************************************************
* FUNCTIONS
******************************************************************************/
void init_pio(void)
{
    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);
    pio_config_set (RADIO_CH1_SEL_PIO, PIO_PULLUP);
    pio_config_set (RADIO_CH2_SEL_PIO, PIO_PULLUP);
    pio_config_set (RADIO_CH3_SEL_PIO, PIO_PULLUP);
    pio_config_set (RADIO_CH4_SEL_PIO, PIO_PULLUP);
    pio_config_set (BUMPER_SWITCH_PIO, PIO_PULLUP);
 
    /* Configure sleep button as input with pullup.  */
    pio_config_set (SLEEP_BUTTON_PIO, PIO_PULLUP);
    pio_irq_config_set (SLEEP_BUTTON_PIO, PIO_IRQ_FALLING_EDGE);

}

void flash_led(int led_pio, int num_flash)
{
    for(int i = 0; i < num_flash; i++)
    {
        pio_output_high (led_pio);
        delay_ms (250);
        pio_output_low (led_pio);
        delay_ms (250);
    }
}

void green_strip(void)
{
    uint8_t leds[NUM_LEDS * 3];
    int i;

    for (i = 0; i < NUM_LEDS; i++)
    {
        // Set full green  GRB order
        leds[i * 3] = 255;
        leds[i * 3 + 1] = 0;
        leds[i * 3 + 2] = 0;
    }

    ledtape_write (LEDTAPE_PIO, leds, NUM_LEDS * 3);
}

void red_strip(void)
{
    uint8_t leds[NUM_LEDS * 3];
    int i;

    for (i = 0; i < NUM_LEDS; i++)
    {
        // Set full green  GRB order
        leds[i * 3] = 0;
        leds[i * 3 + 1] = 255;
        leds[i * 3 + 2] = 0;
    }

    ledtape_write (LEDTAPE_PIO, leds, NUM_LEDS * 3);
}
void blue_strip(void)
{
    uint8_t leds[NUM_LEDS * 3];
    int i;

    for (i = 0; i < NUM_LEDS; i++)
    {
        // Set full green  GRB order
        leds[i * 3] = 0;
        leds[i * 3 + 1] = 0;
        leds[i * 3 + 2] = 255;
    }

    ledtape_write (LEDTAPE_PIO, leds, NUM_LEDS * 3);
}