#include <stdio.h>
#include "usb_serial.h"
#include "pio.h"
#include "delay.h"

#define HELLO_DELAY_MS 500

int main (void)
{
    int i = 0;

    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);

    // Redirect stdio to USB serial    
    usb_serial_stdio_init ();

    while (1)
    {
        delay_ms (HELLO_DELAY_MS);
        
        printf ("Hello world %d\n", i++);

        pio_output_toggle(LED1_PIO);
    }
}
