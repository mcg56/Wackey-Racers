/** @file   usb_racer.h
    @author E. Twomey, M. Gardyne
    @date   11 May 2022
    @brief  USB command functions - header file
    @note   -
*/

#ifndef USB_RACER_H
#define USB_RACER_H


/******************************************************************************
* INCLUDED FILES
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "pio.h"
#include "pwm.h"
#include "panic.h"
#include "delay.h"
#include "usb_serial.h"


/******************************************************************************
* FUNCTIONS
******************************************************************************/

usb_serial_t *
init_usb (void);

#endif /* USB_RACER_H */