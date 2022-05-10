#include "target.h"
#include "motors.h"
#include "pacer.h"
#include "radio.h"

#define LED_FLASH_RATE 10
#define PACER_RATE 1000

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

static usb_serial_cfg_t usb_serial_cfg =
{
    .read_timeout_us = 1,
    .write_timeout_us = 1,
};

static usb_serial_t *usb_serial;


static void
prompt_command (void)
{
    usb_serial_puts (usb_serial, "> ");
}


static void
process_command (void)
{
    char buffer[80];
    char *str;
    
    str = usb_serial_gets (usb_serial, buffer, sizeof (buffer));
    if (! str)
        return;

    //usb_serial (usb_serial, "<<<%s>>>\n", str);
    //usb_serial_puts (usb_serial, str);
    //usb_serial_puts (usb_serial, str[1]);
    

    switch (str[0])
    {
    case 'f':
        usb_serial_puts (usb_serial, "Forward!\n");
        set_motor_vel (0, 70);
        break;

    case 'b':
        usb_serial_puts (usb_serial, "Backward!\n");
        set_motor_vel (0, -50);
        break;
    case 'l':
        usb_serial_puts (usb_serial, "Left!\n");
        set_motor_vel (20, 50);
        break;
        
    case 'r':
        usb_serial_puts (usb_serial, "Right!\n");
        set_motor_vel (-20, 50);
        break;

    case 's':
        usb_serial_puts (usb_serial, "Stop!\n");
        set_motor_vel (0, 0);
        break;

    default:
       break;
    }

    usb_serial_puts (usb_serial, "> ");
}


int
main (void)
{
    //Milesetone 2 butchery
    usb_cdc_t usb_cdc;
    int flash_ticks = 0;
    int i;
    usb_serial = usb_serial_init (&usb_serial_cfg, "");

    prompt_command ();
    
    
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pacer_init (LED_FLASH_RATE * 2);
    //uint8_t i = 0;
    //---------------------Peripheral setup---------------------
    init_pwm (); // Initalise PWM outputs


    // LED SHIT
    //pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    //pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_HIGH);
    //pacer_init (LED_FLASH_RATE*2);

    // Redirect stdio to USB serial
    //usb_serial_stdio_init ();
    set_motor_vel (0, 0);

    while (1) {
        pacer_wait ();
        flash_ticks++;
        if (flash_ticks >= PACER_RATE / (LED_FLASH_RATE * 2)) {
            flash_ticks = 0;

	        pio_output_toggle (LED_STATUS_PIO);

            //process_command ();

            
        }
        
	}
}
