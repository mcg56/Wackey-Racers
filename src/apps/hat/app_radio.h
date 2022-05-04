/** @file   app_radio.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to the nrf24 radio peripheral
*/

#ifndef APP_RADIO_H
#define APP_RADIO_H

/******************************************************************************
* CONSTANTS
******************************************************************************/


/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
int determine_radio_channel(void);

#endif //APP_RADIO_H