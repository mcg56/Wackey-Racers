/** @file   racer_adc.h
    @author M. Gardyne E. Twomey
    @date   10/05/2022
    @brief  Functions and structs relating to reading and processing ADCs
            battery voltage, usb voltage
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
#define NUM_ADC_CHANNELS    1
#define ADC_VREF            3.3
#define ADC_BITS            12
#define BAT_MIN_VOLTAGE     5
#define V_DIV_FACTOR        2.5
#define INITIALISATION_ERROR 2

/******************************************************************************
* GLOBALS
******************************************************************************/
bool low_bat_flag;

/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
void read_adc(adc_t adc, uint16_t *data, int size);
adc_t initialise_adc(void);

#endif //APP_ADC_H