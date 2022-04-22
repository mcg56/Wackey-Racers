/* File:   ledflash1.c
   Author: M. P. Hayes, UCECE
   Date:   15 May 2007
   Descr:  Flash an LED
*/
#include <pio.h>
#include "target.h"
#include "pacer.h"

/* Define LED flash rate in Hz.  */
#define LED_FLASH_RATE 10

/*
    This test app is the faithful blinky program.  It works as follows:
    1. set the LED pin as an output low (turns on the LED if LED active low).
    2. initialize a pacer for 200 Hz.
    3. wait for the next pacer tick.
    4. toggle the LED.
    5. go to step 3.

    Suggestions:
    * Add more LEDs.
    * Blink interesting patterns (S-O-S for example).
    * Make two LEDs blink at two separate frequencies.
*/

int
main (void)
{
    /* Configure STATUS LED PIO as output.  */
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);

    pacer_init (LED_FLASH_RATE * 2);
    int toggle = 0;

    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        /* Toggle LED.  */
        pio_output_toggle (LED_STATUS_PIO);
        toggle += 1;
        if (toggle > 50) {
            pio_output_high (LED_ERROR_PIO);
        } else {
            pio_output_low (LED_ERROR_PIO);
        }
        if (toggle > 100) {
            toggle = 0;
        }
    }
}
