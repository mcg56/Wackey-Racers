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
#include "racer_adc.h"
#include "ledtape.h"
#include "ledbuffer.h"

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
    //nrf24_t *nrf;
    adc_t adc;
    uint16_t adc_data[NUM_ADC_CHANNELS]; // For storing adc data


    //---------------------Initialise Modules ---------------------
    init_pwm (); // Initalise PWM outputs
    init_pio (); // Initalise PIO
    adc = initialise_adc ();
    //nrf = initialise_radio (); // Initalise Radio
    initialise_radio (); // Initalise Radio
    usb_serial_t *usb_serial_1 = init_usb ();  // Initialise USB serial connection


    ledbuffer_t* leds = ledbuffer_init (LEDTAPE_PIO, NUM_LEDS);




    // ---------------------Setting up sleep----------------------------
    const mcu_sleep_wakeup_cfg_t sleep_wake_cfg = {  
        .pio = SLEEP_BUT_PIO,
        .active_high = false
    };
    if(!mcu_sleep_wakeup_set(&sleep_wake_cfg)) panic (LED_ERROR_PIO, INITIALISATION_ERROR);
    

    const mcu_sleep_cfg_t sleep_cfg = {  
        .mode = MCU_SLEEP_MODE_SLEEP //MCU_SLEEP_MODE_WAIT
    };




    // Status LED on
    //pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    
    // Set Motor values to zero
    //set_motor_vel (0, 0); // Start with motor off

    pacer_init (PACER_RATE); 
    int32_t ticks = 0;
    int count_led = 0;

    while (1) {

        ticks++;
        /* Wait until next clock tick.  */
        pacer_wait ();

        /* Run motor.  */ 
        //usb_to_motor (usb_serial_1);
        
        // Radio debug
        //printf("%d\n", nrf24_cfg.channel);


        if (count_led++ == NUM_LEDS)
        {
            // wait for a revolution
            ledbuffer_clear(leds);
            if (blue)
            {
                ledbuffer_set(leds, 0, 0, 0, 255);
                ledbuffer_set(leds, NUM_LEDS / 2, 0, 0, 255);
            }
            else
            {
                ledbuffer_set(leds, 0, 255, 0, 0);
                ledbuffer_set(leds, NUM_LEDS / 2, 255, 0, 0);
            }
            blue = !blue;
            count_led = 0;
        }
        ledbuffer_write (leds);
        ledbuffer_advance (leds, 1);






        pio_output_toggle (LED_STATUS_PIO);
        read_adc(adc, adc_data, sizeof(adc_data));

        if (low_bat_flag)
        { 
            pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_HIGH);
        } else 
        {
            pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
        }
        
        if (!pio_input_get (BUMPER_SWITCH_PIO)) 
        {
            radio_transmit();
            set_motor_vel (101, 101);
            delay_ms(5000); 
            
        } else {
            if (ticks > 5) 
            {
                ticks = 0;   
                radio_recieve();
            }
            
        }
	}
}
