#include <stdio.h>
#include "usb_serial.h"
#include "pio.h"
#include "sys.h"
#include "pacer.h"
#include "delay.h"

#define HELLO_DELAY 500

int main (void)
{
    int i = 0;

    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_HIGH);

    // Redirect stdio to USB serial    
    usb_serial_stdio_init ();

    while (1)
    {
        delay_ms (HELLO_DELAY);
        
        printf ("Hello world %d\n", i++);
        fflush (stdout);

        pio_output_toggle(LED1_PIO);
        pio_output_toggle(LED2_PIO);
    }
}
