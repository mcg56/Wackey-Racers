/* File:   radio_rx_test1.c
   Author: M. P. Hayes, UCECE
   Date:   24 Feb 2018
   Descr:
*/
#include "nrf24.h"
#include "usb_serial.h"
#include "pio.h"
#include "delay.h"


static void panic (void)
{
    while(1)
    {
        pio_output_toggle (LED1_PIO);
        pio_output_toggle (LED2_PIO);
        delay_ms (400);
    }
}

int main(void)
{
    nrf24_cfg_t nrf24_cfg =
        {
            .ce_pio = RADIO_CE_PIO,
            .irq_pio = RADIO_IRQ_PIO,
            .channel = 4,
            .address = 0x0123456789,
            .spi =
            {
                .channel = 0,
                .clock_speed_kHz = 1000,
                .cs = RADIO_CS_PIO,
                .mode = SPI_MODE_0,
                .cs_mode = SPI_CS_MODE_FRAME,
                .bits = 8,
            }
        };
    nrf24_t *nrf;
    usb_cdc_t usb_cdc;

    // Configure LED PIO as output.
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);

    // Redirect stdio to USB serial.
    usb_serial_stdio_init ();

#ifdef RADIO_PWR_EN
    // Enable radio regulator if present.
    pio_config_set (RADIO_PWR_EN, PIO_OUTPUT_HIGH);
    delay_ms (10);
#endif

    nrf = nrf24_init (&nrf24_cfg);
    if (! nrf)
        panic ();

    while(1)
    {
        char buffer[32];

        if (nrf24_read (nrf, buffer, sizeof (buffer)))
        {
            printf ("%s\n", buffer);
            pio_output_toggle (LED2_PIO);
            pio_output_toggle (LED1_PIO);
        }
    }
}
