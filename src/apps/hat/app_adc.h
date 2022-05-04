/** @file   app_adc.h
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to reading and processing ADCs
            e.g joystick, battery voltage, usb voltage
*/

#ifndef APP_ADC_H
#define APP_ADC_H

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "adc.h"

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define NUM_ADC_CHANNELS 3

/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
void task_read_adc(adc_t adc, uint16_t *data, int size);

#endif //APP_ADC_H