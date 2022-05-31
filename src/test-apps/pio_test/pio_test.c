#include "mcu.h"
#include "pio.h"
#include "pacer.h"

#define LED_FLASH_RATE 10

int
main (void)
{
    
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (DARSTEDLY_OUTPUT_1_PIO, PIO_OUTPUT_LOW);

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
            pio_output_high (DARSTEDLY_OUTPUT_1_PIO);
        } else {
            pio_output_low (DARSTEDLY_OUTPUT_1_PIO);
        }
        if (toggle > 100) {
            toggle = 0;
        }
    }
}