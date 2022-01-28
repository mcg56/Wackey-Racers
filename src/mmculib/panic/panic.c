#include "pio.h"
#include "delay.h"

void
panic (pio_t led_error_pio, unsigned int errorcode)
{
    unsigned int i;

    pio_config_set (led_error_pio, PIO_OUTPUT_LOW);

    while (1)
    {
        for (i = 0; i < errorcode; i++)
        {
            pio_output_toggle (led_error_pio);
            delay_ms (200);
            pio_output_toggle (led_error_pio);
            delay_ms (200);
        }
        delay_ms (1000);
    }
}
