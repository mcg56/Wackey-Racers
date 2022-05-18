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
#define PACER_RATE 50 //Hz



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
- Bat adc
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
    int32_t ticks = 0;
    int32_t ticks_1 = 0;

    while (1) {

        ticks++;
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
        pio_output_toggle (LED_STATUS_PIO);
        
        if (!pio_input_get (BUMPER_SWITCH_PIO)) 
        {
            tx_buffer[0] = 1 & 0xFF;
            ticks_1 = 0;
            while (ticks_1 <= 5) 
            {
                nrf24_write (nrf, tx_buffer, RADIO_TX_PAYLOAD_SIZE);
                ticks_1++;
            }
            set_motor_vel (101, 101);
            delay_ms(5000);
            
            
        } else {
            if (ticks > 5) 
            {
                ticks = 0;   
                rx_bytes = nrf24_read (nrf, rx_buffer, RADIO_RX_PAYLOAD_SIZE); // Maybe buffer needs to be 3 long same as tx...
                if (rx_bytes != 0)
                {
                    rx_buffer[rx_bytes] = 0;
                    
                    //printf("%d %d %d\n", rx_buffer[0], rx_buffer[1], rx_buffer[2]);
                    set_motor_vel ((int)rx_buffer[0], (int)rx_buffer[1]);
                }
            }
            
        }
	}
}
