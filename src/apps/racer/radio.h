/* @file   radio.c
    @author E. Twomey, M. Gardyne
    @date   4 May 2022
    @brief  Radio Recieve and Transmit
    @note   Needs completed
*/

#ifndef RADIO_H
#define RADIO_H

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "nrf24.h"
#include "spi.h"
#include "usb_serial.h"
#include "pio.h"
#include "delay.h"
#include "panic.h"

#define RADIO_ADDRESS           0x0123456789LL
#define RADIO_TX_PAYLOAD_SIZE   3
#define RADIO_RX_PAYLOAD_SIZE   3
#define RADIO_CHANNEL_DEFAULT   0

int determine_radio_channel(void);
nrf24_t *initialise_radio(void);

nrf24_cfg_t nrf24_cfg;

#endif /* RADIO_H */