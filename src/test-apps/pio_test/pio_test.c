#include "mcu.h"
#include "pio.h"
#include "pacer.h"

#define PACER_RATE 100

int
main (void)
{
    /* Configure LED PIO as output.  */
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEVEL_SHIFTER_2_3V3, PIO_OUTPUT_LOW);

    /* Configure button PIO as input with pullup.  */
    pio_config_set (BUTTON_PIO, PIO_PULLUP);

    pacer_init (PACER_RATE);

    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        if (pio_input_get (BUTTON_PIO)){
            pio_output_low (LED_STATUS_PIO);
            pio_output_low (LEVEL_SHIFTER_2_3V3);}
        else{
            pio_output_high (LED_STATUS_PIO);
            pio_output_high (LEVEL_SHIFTER_2_3V3);}
    }
}