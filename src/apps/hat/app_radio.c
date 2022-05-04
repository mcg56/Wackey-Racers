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