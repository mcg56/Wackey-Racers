/* File:   button_test1.c
   Author: M. P. Hayes, UCECE
   Date:   18 Dec 2021
   Descr:  Simple button test demo without debouncing
*/
#include "mcu.h"
#include "pio.h"
#include "pacer.h"

#define PACER_RATE 100

int
main (void)
{
    mcu_jtag_disable(); 
    /* Configure LED PIO as output.  */
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    /* Configure button PIO as input with pullup.  */
    pio_config_set (SLEEP_BUT_PIO, PIO_PULLUP);

    pacer_init (PACER_RATE);

    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        if (pio_input_get (SLEEP_BUT_PIO))
            pio_output_high (LED_STATUS_PIO);
        else
            pio_output_low (LED_STATUS_PIO);
    }
}
