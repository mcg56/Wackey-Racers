#include "target.h"
#include "motors.h"
#include "pacer.h"

#define LED_FLASH_RATE 10

/* TODO
- Always specify size eg (uint8_t)
- Typedef enum for motor states (DONE)
- Cut out if no packet recieved after ... amount of time
- Data packets 3 bytes (1 linear, 1 turn, 1 darstedly)
- Potential radio authentication? could be difficult (something like parity)
- Bumper clear send packet
- Broadcast many packets when bumper hit to ensure hat recieves
- Three point throttle curve
*/



int
main (void)
{
    uint8_t i = 0;
    //---------------------Peripheral setup---------------------
    init_pwm (); // Initalise PWM outputs


    // LED SHIT
    //pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    //pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_HIGH);
    //pacer_init (LED_FLASH_RATE*2);

    // Redirect stdio to USB serial
    usb_serial_stdio_init ();
    set_motor_vel (20, 70);

    while (1) {
        //pacer_wait ();
        //set_motor_vel (0, 100);
        //pio_output_toggle (LED_STATUS_PIO);
        //pio_output_toggle (LED_ERROR_PIO);
        //printf ("Hello world %d\n", i++);
    }
}
