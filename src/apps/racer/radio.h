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

int radio_recieve (void);
int radio_transmit (void);

#endif /* RADIO_H */