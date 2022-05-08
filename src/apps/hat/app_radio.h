/** @file   app_radio.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to the nrf24 radio peripheral
*/

#ifndef APP_RADIO_H
#define APP_RADIO_H

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "nrf24.h"

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define RADIO_ADDRESS           0x0123456789LL
#define RADIO_PAYLOAD_SIZE      3
#define RADIO_CHANNEL_DEFAULT   0
/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
int determine_radio_channel(void);
nrf24_t *initialise_radio(void);

#endif //APP_RADIO_H