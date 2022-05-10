/* @file   radio.c
    @author E. Twomey, M. Gardyne
    @date   4 May 2022
    @brief  Radio Recieve and Transmit
    @note   Needs completed
*/

#include "radio.h"


#define RADIO_CHANNEL 1
#define RADIO_ADDRESS 0x0123456789FF
#define RADIO_PAYLOAD_SIZE 3



int radio_recieve (void)
{
    spi_cfg_t spi_cfg =
        {
            .channel = 0,
            .clock_speed_kHz = 1000,
            .cs = RADIO_CS_PIO,
            .mode = SPI_MODE_0,
            .cs_mode = SPI_CS_MODE_FRAME,
            .bits = 8
        };
    nrf24_cfg_t nrf24_cfg =
        {
            .channel = RADIO_CHANNEL,
            .address = RADIO_ADDRESS,
            .payload_size = RADIO_PAYLOAD_SIZE,
            .ce_pio = RADIO_CE_PIO,
            .irq_pio = RADIO_IRQ_PIO,
        };
    spi_t spi;
    nrf24_t *nrf;

    // Configure LED PIO as output.                         - DONE IN MAIN
    //pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
    //pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    // Redirect stdio to USB serial.
    usb_serial_stdio_init ();

#ifdef RADIO_POWER_ENABLE_PIO
    // Enable radio regulator if present.
    pio_config_set (RADIO_POWER_ENABLE_PIO, PIO_OUTPUT_HIGH);
    delay_ms (10);
#endif

    spi = spi_init ( &spi_cfg);
    if (! spi)
        panic (LED_ERROR_PIO, 1);

    nrf = nrf24_init (spi, &nrf24_cfg);
    if (! nrf)
        panic (LED_ERROR_PIO, 2);

    while(1)
    {
        char buffer[RADIO_PAYLOAD_SIZE + 1];
        uint8_t bytes;

        bytes = nrf24_read (nrf, buffer, RADIO_PAYLOAD_SIZE);
        if (bytes != 0)
        {
            buffer[bytes] = 0;
            printf ("%i %i %i\n", buffer[0], buffer[1], buffer[2]);
            pio_output_toggle (LED_STATUS_PIO);
        }
    }
}


int radio_transmit (void)
{
    spi_cfg_t spi_cfg =
        {
            .channel = 0,
            .clock_speed_kHz = 1000,
            .cs = RADIO_CS_PIO,
            .mode = SPI_MODE_0,
            .cs_mode = SPI_CS_MODE_FRAME,
            .bits = 8
        };
    nrf24_cfg_t nrf24_cfg =
        {
            //.channel = RADIO_CHANNEL,
            .address = RADIO_ADDRESS,
            .payload_size = RADIO_PAYLOAD_SIZE,
            .ce_pio = RADIO_CE_PIO,
            .irq_pio = RADIO_IRQ_PIO,
        };
    uint8_t count = 0;
    spi_t spi;
    nrf24_t *nrf;

    // Configure LED PIO as output.
    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pacer_init (10);

#ifdef RADIO_POWER_ENABLE_PIO
    // Enable radio regulator if present.
    pio_config_set (RADIO_POWER_ENABLE_PIO, PIO_OUTPUT_HIGH);
    delay_ms (10);
#endif

    spi = spi_init ( &spi_cfg);
    if (! spi) panic (LED_ERROR_PIO, 1);

    nrf = nrf24_init (spi, &nrf24_cfg);
    if (! nrf) panic (LED_ERROR_PIO, 2);

    while (1)
    {
        char buffer[RADIO_PAYLOAD_SIZE + 1];

        pacer_wait ();
        pio_output_toggle (LED_STATUS_PIO);

        snprintf (buffer, sizeof (buffer), "EAT MY HAIRY ASS\r\n");

        if (! nrf24_write (nrf, buffer, RADIO_PAYLOAD_SIZE)) {
            pio_output_set (LED_ERROR_PIO, 0);
        } else {
            pio_output_set (LED_ERROR_PIO, 1);
        }
    }
}


int write_to_radio (void)
{
    snprintf (buffer, sizeof (buffer), "Message\r\n");
}