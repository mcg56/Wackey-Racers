/** @file   app_init.h
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to the initialisation
*/

#ifndef APP_INIT_H
#define APP_INIT_H

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "pio.h"
#include "delay.h"
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "mpu9250.h"
#include "panic.h"
#include "mcu.h"
#include "app_gpio.h"
#include "adc.h"
#include "nrf24.h"
#include "spi.h"
#include "usb_serial.h"
#include "delay.h"
#include "string.h"

/******************************************************************************
* CONSTANTS
******************************************************************************/


/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
mpu_t *initialise_imu(void);
adc_t initialise_adc(void);
nrf24_t *initialise_radio(void);

#endif //APP_INIT_H
