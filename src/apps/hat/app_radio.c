/** @file   app_radio.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to the nrf24 radio peripheral
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "target.h"
#include "hat.h"
#include "pio.h"
#include "panic.h"
#include "nrf24.h"
#include "spi.h"
#include "app_radio.h"

/******************************************************************************
* FUNCTIONS
******************************************************************************/
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

nrf24_t *initialise_radio(void)
{
    uint8_t count = 0; //Radio Variables
    spi_t spi;
    nrf24_t *nrf;
    int radio_channel;
    // Initialise Radio
    spi = spi_init ( &spi_cfg);
    if (! spi) panic (LED_ERROR_PIO, 1);

    nrf = nrf24_init (spi, &nrf24_cfg);
    if (! nrf) panic (LED_ERROR_PIO, 2);

    radio_channel = determine_radio_channel();
    printf("Radio channel: %d\n", radio_channel);

    return nrf;
}



int determine_radio_channel(void)
{
    int radio_channel = -1;
    int num_channels_found = 0;

    if (!pio_input_get (CH1_SEL)) 
    {
        radio_channel = 1;
        num_channels_found++;
    }
    if (!pio_input_get (CH2_SEL)) 
    {
        radio_channel = 2;
        num_channels_found++;
    }
    if (!pio_input_get (CH3_SEL)) 
    {
        radio_channel = 3;
        num_channels_found++;
    }
    if (!pio_input_get (CH4_SEL)) 
    {
        radio_channel = 4;
        num_channels_found++;
    }

    if (num_channels_found > 1) //More than 1 channel select is in, only have 1 
    {
        printf("Error: Too many radio channels selected!");
        panic (LED_ERROR_PIO, RADIO_CHANNEL_SEL_ERROR);
    }
    
    if(num_channels_found == 0) //No channel selected
    {
        printf("Error: No channel selected!");
        panic (LED_ERROR_PIO, RADIO_CHANNEL_SEL_ERROR);
    }

    return radio_channel;
}


