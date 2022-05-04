/** @file   app_adc.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to reading and processing ADCs
            e.g joystick, battery voltage, usb voltage
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "target.h"
#include "hat.h"
#include "pio.h"
#include "app_adc.h"