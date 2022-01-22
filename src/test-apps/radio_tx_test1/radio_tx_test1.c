/* File:   radio_tx_test1.c
   Author: M. P. Hayes, UCECE
   Date:   24 Feb 2018
*/
#include "nrf24.h"
#include "pio.h"
#include "pacer.h"
#include "stdio.h"
#include "delay.h"

#define RADIO_CHANNEL 4
#define RADIO_ADDRESS 0x0123456789LL

static void panic (void)
{
    while (1)
    {
        pio_output_toggle (LED1_PIO);
        pio_output_toggle (LED2_PIO);
        delay_ms (400);
    }
}

int main (void)
{
    nrf24_cfg_t nrf24_cfg =
        {
            .channel = RADIO_CHANNEL,
            .address = RADIO_ADDRESS,
            .ce_pio = RADIO_CE_PIO,
            .irq_pio = RADIO_IRQ_PIO,
            .spi =
            {
                .channel = 0,
                .clock_speed_kHz = 1000,
                .cs = RADIO_CS_PIO,
                .mode = SPI_MODE_0,
                .cs_mode = SPI_CS_MODE_FRAME,
                .bits = 8
            }
        };
    uint8_t count = 0;
    nrf24_t *nrf;

    // Configure LED PIO as output.
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_HIGH);
    pacer_init (10);

#ifdef RADIO_PWR_EN
    // Enable radio regulator if present.
    pio_config_set (RADIO_PWR_EN, PIO_OUTPUT_HIGH);
    delay_ms (10);
#endif

    nrf = nrf24_init (&nrf24_cfg);
    if (! nrf)
        panic ();

    while (1)
    {
        char buffer[32];

        pacer_wait ();
        pio_output_toggle (LED2_PIO);

        sprintf (buffer, "Hello world %d\r\n", count++);

        if (! nrf24_write (nrf, buffer, sizeof (buffer)))
            pio_output_set (LED1_PIO, 0);
        else
            pio_output_set (LED1_PIO, 1);
    }
}
