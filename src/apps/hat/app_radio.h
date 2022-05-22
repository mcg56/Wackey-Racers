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
#define RADIO_ADDRESS           0x0123456789LL//0x2123456789
#define RADIO_TX_PAYLOAD_SIZE   3
#define RADIO_RX_PAYLOAD_SIZE   3
#define RADIO_CHANNEL_DEFAULT   0
/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
int determine_radio_channel(void);
nrf24_t *initialise_radio(void);
nrf24_cfg_t nrf24_cfg;

extern spi_t spi;
extern nrf24_t *nrf;
extern spi_cfg_t spi_cfg;
#endif //APP_RADIO_H