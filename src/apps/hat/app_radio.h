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


/******************************************************************************
* CONSTANTS
******************************************************************************/
#define RADIO_ADDRESS 0x0123456789LL
#define RADIO_PAYLOAD_SIZE 3

/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
int determine_radio_channel(void);

#endif //APP_RADIO_H