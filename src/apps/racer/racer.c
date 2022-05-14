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
#include "racer_pio.h"

/******************************************************************************
* GLOBAL VARIABLES
******************************************************************************/
#define PACER_RATE 100 //Hz



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
- Sleep
*/

/******************************************************************************
* KERNEL
******************************************************************************/

int main (void)
{
    //---------------------Variables---------------------
    nrf24_t *nrf;


    //---------------------Initialise Modules ---------------------
    init_pwm (); // Initalise PWM outputs
    init_pio (); // Initalise PIO
    nrf = initialise_radio (); // Initalise Radio
    usb_serial_t *usb_serial_1 = init_usb ();  // Initialise USB serial connection


    // Status LED on
    //pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    
    // Set Motor values to zero
    //set_motor_vel (0, 0); // Start with motor off

    pacer_init (PACER_RATE); 

    while (1) {

        /* Wait until next clock tick.  */
        pacer_wait ();

        /* Radio  */
        char tx_buffer[RADIO_TX_PAYLOAD_SIZE + 1]; // +1 for null terminator
        char rx_buffer[RADIO_RX_PAYLOAD_SIZE + 1]; // +1 for null terminator
        uint8_t rx_bytes;
        
        /* Run motor.  */ 
        //usb_to_motor (usb_serial_1);
        
        // Radio debug
        //printf("%d\n", nrf24_cfg.channel);


        /******************************************************************************
        * Radio
        ******************************************************************************/
        
        
        if (!pio_input_get (BUMPER_SWITCH_PIO)) 
        {
            tx_buffer[0] = 1 & 0xFF;
            nrf24_write (nrf, tx_buffer, RADIO_TX_PAYLOAD_SIZE);
            pio_output_toggle (LED_ERROR_PIO);
        } else {
            rx_bytes = nrf24_read (nrf, rx_buffer, RADIO_RX_PAYLOAD_SIZE); // Maybe buffer needs to be 3 long same as tx...
            if (rx_bytes != 0)
            {
                rx_buffer[rx_bytes] = 0;
                //printf ("%s\n", rx_buffer);
                printf("%i %i %i\n", rx_buffer[0], rx_buffer[1], rx_buffer[2]);
                pio_output_toggle (LED_STATUS_PIO);
            }
        }
	}
}
