/* File:   adc_usb_cdc_test2.c
   Author: M. P. Hayes, UCECE
   Date:   3 May 2021
   Descr:  This reads from ADC channels 2 and 4.
           It triggers ADC conversions as each sample is read.
*/
#include <stdio.h>
#include "usb_serial.h"
#include "adc.h"
#include "pacer.h"

#define PACER_RATE 2

static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (ADC_CHANNEL_2) | BIT (ADC_CHANNEL_4),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000
};


int main (void)
{
    usb_cdc_t usb_cdc;
    adc_t adc;
    int count = 0;

    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    adc = adc_init (&adc_cfg);

    pacer_init (PACER_RATE);
    while (1)
    {
        uint16_t data[2];

        pacer_wait ();

        // The lowest numbered channel is read first.
        adc_read (adc, data, sizeof (data));

        printf ("%3d: %d, %d\n", count, data[0], data[1]);
    }
}
