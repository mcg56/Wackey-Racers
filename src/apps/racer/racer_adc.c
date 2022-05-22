/** @file   racer_adc.c
    @author M.Gardyne E.Twomey
    @date   10/05/2022
    @brief  Functions and structs relating to reading and processing ADCs
            e.g joystick, battery voltage, usb voltage
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "target.h"
#include "pio.h"
#include "racer_adc.h"
#include "adc.h"
#include "panic.h"
#include "math.h"

/******************************************************************************
* GLOBALS
******************************************************************************/
bool low_bat_flag = false;

/******************************************************************************
* FUNCTIONS
******************************************************************************/
static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (BATTERY_ADC_CHANNEL),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000
};

adc_t initialise_adc(void)
{
    adc_t adc;
    // Initialise the ADC
    adc = adc_init (&adc_cfg);
    if (! adc) panic (LED_ERROR_PIO, INITIALISATION_ERROR);
    return adc;
};

void read_adc(adc_t adc, uint16_t *data, int size)
{
    // The lowest numbered channel is read first.
    adc_read (adc, data, size);

    if (data[0] < BAT_MIN_VOLTAGE*(pow(2, ADC_BITS)-1)/ADC_VREF/V_DIV_FACTOR)
    {
        low_bat_flag = true;
    } else
    {
        low_bat_flag = false;
    }
}