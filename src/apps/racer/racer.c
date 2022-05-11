/** @file   racer.c
    @author E. Twomey, M. Gardyne
    @date   11 May 2022
    @brief  Kernel
    @note   -
*/


/******************************************************************************
* INCLUDES
******************************************************************************/

#include "target.h"
#include "motors.h"
#include "pacer.h"
#include "radio.h"
#include "usb_racer.h"



/******************************************************************************
* GLOBAL VARIABLES
******************************************************************************/

#define LED_FLASH_RATE 10
#define PACER_RATE 1000



/******************************************************************************
* TODO
******************************************************************************/
/*
- Always specify size eg (uint8_t)
- Typedef enum for motor states (DONE)
- Cut out if no packet recieved after ... amount of time
- Data packets 3 bytes (1 linear, 1 turn, 1 darstedly)
- Potential radio authentication? could be difficult (something like parity)
- Bumper clear send packet
- Broadcast many packets when bumper hit to ensure hat recieves
- Three point throttle curve
*/



/******************************************************************************
* KERNEL
******************************************************************************/

int
main (void)
{
    // Status LED on
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    // Set pacer period
    pacer_init (LED_FLASH_RATE * 2);


    // Milesetone 2 variables
    nrf24_t *nrf;
    int rx_bytes;
    int flash_ticks = 0;


    //---------------------Peripheral setup---------------------
    init_pwm (); // Initalise PWM outputs
    usb_serial_t *usb_serial_1 = init_usb ();  // Initialise USB serial connection


    // Redirect stdio to USB serial
    //usb_serial_stdio_init ();

    set_motor_vel (0, 0); // Start with motor off



    while (1) {

        /* Radio  */
        char tx_buffer[RADIO_TX_PAYLOAD_SIZE + 1]; // +1 for null terminator
        char rx_buffer[RADIO_RX_PAYLOAD_SIZE + 1]; // +1 for null terminator

        /* Wait until next clock tick.  */
        pacer_wait ();

        /* Flash the status LED.  */
        flash_ticks++;
        if (flash_ticks >= PACER_RATE / (LED_FLASH_RATE * 2)) {
            flash_ticks = 0;

	        pio_output_toggle (LED_STATUS_PIO);

            /* Run motor.  */
            usb_to_motor (usb_serial_1);   
        }






        /******************************************************************************
        * TODO - Get the radio working.
        ******************************************************************************/

        /*
        tx_buffer[0] = "H"; 
        // if (! nrf24_write (nrf, tx_buffer, RADIO_TX_PAYLOAD_SIZE)) {
        //     pio_output_set (LED_ERROR_PIO, 1);
        // } else {
        //     pio_output_set (LED_ERROR_PIO, 0);
        // }
        rx_bytes = nrf24_read (nrf, rx_buffer, RADIO_RX_PAYLOAD_SIZE); // Maybe buffer needs to be 3 long same as tx...
        if (rx_bytes != 0)
        {
            rx_buffer[rx_bytes] = 0;
            printf ("%s\n", rx_buffer);
            pio_output_toggle (LED_ERROR_PIO);
        }
        printf("Radio channel: %d\n", nrf24_cfg.channel);
        printf("Radio channel: %d\n", RADIO_ADDRESS);
        */
	}
}
