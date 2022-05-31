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
#include "mcu_sleep.h"
#include "pio.h"
#include "irq.h"

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
        .pio = SLEEP_BUTTON_PIO,
        .active_high = false
    };
    
    if(!mcu_sleep_wakeup_set(&sleep_wake_cfg)) panic (LED_ERROR_PIO, INITIALISATION_ERROR);
    

    const mcu_sleep_cfg_t sleep_cfg = {  
        .mode = MCU_SLEEP_MODE_BACKUP //MCU_SLEEP_MODE_WAIT
    };
    



    // Status LED on
    //pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    
    // Set Motor values to zero
    //set_motor_vel (0, 0); // Start with motor off

    pacer_init (PACER_RATE); 
    int32_t ticks = 0;
    int count_led = 0;
    bool blue = false;
    int8_t turn_direction;

    while (1) {

        ticks++;
        /* Wait until next clock tick.  */
        pacer_wait ();

        /* Run motor.  */ 
        //usb_to_motor (usb_serial_1);
        
        // Radio debug
        //printf("%d\n", nrf24_cfg.channel);

        /*
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


        if (count_led++ == 4)
        {
            ledbuffer_clear(leds);
            if (blue)
            {
                for (int led_num = 0; led_num < 12; led_num++) {
                    ledbuffer_set(leds, 0, 0, 255, 0);
                }
            }
            else
            {
                ledbuffer_clear(leds);
            }
        }
        */

        pio_output_toggle (LED_STATUS_PIO);
        read_adc(adc, adc_data, sizeof(adc_data));

        if (low_bat_flag)
        { 
            pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_HIGH);
            empty_strip();
            pio_config_set(MOTOR_ENABLE_PIO, PIO_OUTPUT_LOW);
        } else 
        {
            pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
            pio_config_set(MOTOR_ENABLE_PIO, PIO_OUTPUT_HIGH);
        }
        
        if (!pio_input_get (BUMPER_SWITCH_PIO)) 
        {
            radio_transmit();
            set_motor_vel (101, 101);

            int flash_times = 0;
            while(flash_times < 5)
                {
                    empty_strip();
                    delay_ms(500);
                    red_strip();
                    delay_ms(500);
                    flash_times++;
                }
            set_motor_vel (101, 1);
            delay_ms(1000); 
            set_motor_vel (101, 101);
            
        } else {
            turn_direction = radio_recieve();
        }

        green_strip();
        // if (turn_direction == TURN_LEFT) {
        //     ledbuffer_set(leds, 5, 200, 50, 0);
        //     ledbuffer_set(leds, 6, 200, 50, 0);
        //     ledbuffer_set(leds, 17, 200, 50, 0);
        //     ledbuffer_set(leds, 18, 200, 50, 0);
        // } else if (turn_direction == TURN_RIGHT) {
        //     ledbuffer_set(leds, 0, 200, 50, 0);
        //     ledbuffer_set(leds, 11, 200, 50, 0);
        //     ledbuffer_set(leds, 12, 200, 50, 0);
        //     ledbuffer_set(leds, 23, 200, 50, 0);
        // } else {
        //     green_strip();
        // }


        // Poll sleep button and if pressed then sleep...
        // if (!pio_input_get (SLEEP_BUTTON_PIO) && (ticks >= 100)) //sleep button pressed
        // {
        //     // Do stuff to show we recieved the button press
        //     red_strip();
        //     pio_output_set (LED_STATUS_PIO, 0);
        //     pio_output_set (LED_ERROR_PIO, 0);

        //     delay_ms(1000);

        //     // Try enable the interrupt
        //     pio_irq_clear(SLEEP_BUTTON_PIO);
        //     pio_irq_enable(SLEEP_BUTTON_PIO);
        //     irq_enable(PIO_ID(SLEEP_BUTTON_PIO));

        //     pio_sleep_mode();   
        //     empty_strip();

        //     //Sleep mcu
        //     mcu_select_slowclock();
        //     mcu_sleep(&sleep_cfg);


        //     //Flash LED to show we wokeup
        //     flash_led(LED_STATUS_PIO, 5);
        //     pio_config_set(SLEEP_BUTTON_PIO, PIO_OUTPUT_HIGH);
        //     ticks = 0;
        // }
	}
}
