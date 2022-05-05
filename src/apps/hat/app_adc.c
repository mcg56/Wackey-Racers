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
#include "adc.h"

/******************************************************************************
* FUNCTIONS
******************************************************************************/

void task_read_adc(adc_t adc, uint16_t *data, int size)
{
    // The lowest numbered channel is read first.
    adc_read (adc, data, size);
    //TO DO:
    // Maybe process data...
    // Compare bat div reading to its nominal value to check if battery voltage is low, raise flag or something if so...

}