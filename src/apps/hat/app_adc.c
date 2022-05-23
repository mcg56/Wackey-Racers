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
#include "panic.h"
#include "math.h"

/******************************************************************************
* GLOBALS
******************************************************************************/
bool low_bat_flag = false;
adc_t adc;

/******************************************************************************
* FUNCTIONS
******************************************************************************/
static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (BATTERY_ADC_CHANNEL) | BIT (JOYSTICK_X_ADC_CHANNEL) | BIT (JOYSTICK_Y_ADC_CHANNEL),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000
};

void initialise_adc(void)
{
    // Initialise the ADC
    adc = adc_init (&adc_cfg);
    if (! adc) panic (LED_ERROR_PIO, INITIALISATION_ERROR);
};

void task_read_adc(adc_t adc, uint16_t *data, int size)
{
    // The lowest numbered channel is read first.
    adc_read (adc, data, size);

    // "Invert" the x joystick reading to match the IMU
    int x_inversion = data[1] - (pow(2, ADC_BITS)-1)/2;
    data[1] = data[1] - 2*x_inversion;

    //TO DO:
    // Maybe process data...
    // Compare bat div reading to its nominal value to check if battery voltage is low, raise flag or something if so...
    if (data[0] < BAT_MIN_VOLTAGE*(pow(2, ADC_BITS)-1)/ADC_VREF/V_DIV_FACTOR)
    {
        low_bat_flag = true;
    } else
    {
        low_bat_flag = false;
    }
}