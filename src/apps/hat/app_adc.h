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
#define NUM_ADC_CHANNELS    3
#define ADC_VREF            3.3
#define ADC_BITS            12
#define BAT_MIN_VOLTAGE     5
#define V_DIV_FACTOR        2.5

#define JOY_LIMIT_BUFFER    200
#define JOY_X_MIN           (200 + JOY_LIMIT_BUFFER)
#define JOY_X_MAX           (4020 - JOY_LIMIT_BUFFER)
#define JOY_Y_MIN           (380 + JOY_LIMIT_BUFFER)
#define JOY_Y_MAX           (4015 - JOY_LIMIT_BUFFER)


/******************************************************************************
* GLOBALS
******************************************************************************/
bool low_bat_flag;

/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
void task_read_adc(adc_t adc, uint16_t *data, int size);
adc_t initialise_adc(void);

#endif //APP_ADC_H