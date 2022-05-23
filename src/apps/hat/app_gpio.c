/** @file   app_gpio.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to general purpose pio 
            (leds, buttons etc)
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "target.h"
#include "hat.h"
#include "pio.h"
#include "app_gpio.h"
#include "delay.h"
#include "ledbuffer.h"
#include "ledtape.h"
#include "irq.h"
#include "app_radio.h"
#include "spi.h"


void wake_isr(void)
{
    //Set clk back to normal and clear interrupt
    mcu_power_mode_normal();
    pio_irq_clear (SLEEP_BUT_PIO);

    // Disable IRQ so that we can sleep again
    pio_irq_disable(SLEEP_BUT_PIO);
    irq_disable(PIO_ID(SLEEP_BUT_PIO));

    //Wake everything back up
    //SPI and radio
    spi = spi_init (&spi_cfg);
    nrf24_power_up (nrf);

    //TWI imu
    initialise_imu();

    delay_ms(500); // Delay to debounce button
}


/******************************************************************************
* FUNCTIONS
******************************************************************************/
void pio_configuration(void)
{
    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);
    pio_config_set (FSYNC, PIO_OUTPUT_LOW);
    pio_config_set (CH1_SEL, PIO_PULLUP);
    pio_config_set (CH2_SEL, PIO_PULLUP);
    pio_config_set (CH3_SEL, PIO_PULLUP);
    pio_config_set (CH4_SEL, PIO_PULLUP);
    pio_config_set (GPIO_JUMPER, PIO_PULLUP);
    /* Configure sleep button as input with pullup.  */
    pio_config_set (SLEEP_BUT_PIO, PIO_PULLUP);
    //pio_irq_config_set (SLEEP_BUT_PIO, PIO_IRQ_LOW_LEVEL);
    irq_config(PIO_ID(SLEEP_BUT_PIO), 1, wake_isr);
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

void any_strip(int r, int g, int b)
{
    uint8_t leds[NUM_LEDS * 3];
    int i;

    for (i = 0; i < NUM_LEDS; i++)
    {
        // Set full green  GRB order
        leds[i * 3] = g;
        leds[i * 3 + 1] = r;
        leds[i * 3 + 2] = b;
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

void empty_strip(void)
{
    {
    uint8_t leds[NUM_LEDS * 3];
    int i;

    for (i = 0; i < NUM_LEDS; i++)
    {
        // Set full green  GRB order
        leds[i * 3] = 0;
        leds[i * 3 + 1] = 0;
        leds[i * 3 + 2] = 0;
    }

    ledtape_write (LEDTAPE_PIO, leds, NUM_LEDS * 3);
    }
}