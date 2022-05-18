/** @file   hat.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Main code for the hat module

    To DO:
    -Sleep (needs to wakeup again - Angus)
    -LED tape (anton)
    -Buzzer somehthing small on hit (anton)
    -buzzer play anthem unblocked (Plays interesting sounds - Anton)
    -disable LED tape when low power
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "target.h"
#include "pio.h"
#include "delay.h"
#include "pacer.h"
#include "usb_serial.h"
#include "mpu9250.h"
#include "panic.h"
#include "mcu.h"
#include "app_radio.h"
#include "app_imu.h"
#include "hat.h"
#include "app_adc.h"
#include "app_pwm.h"
#include "app_gpio.h"
#include "adc.h"
#include "nrf24.h"
#include "spi.h"
#include "string.h"
#include "mcu_sleep.h"
#include "pwm.h"
#include "ledtape.h"
#include "ledbuffer.h"
#include <stdio.h>
#include "math.h"

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define PACER_RATE 50 //Hz


/******************************************************************************
* Globals
******************************************************************************/



int main (void)
{
    //---------------------Variables---------------------
    nrf24_t *nrf;
    adc_t adc;
    mpu_t *mpu;
    int16_t accel[NUM_ACCEL_VALUES]; // For storing imu data
    uint16_t adc_data[NUM_ADC_CHANNELS]; // For storing adc data
    bool use_joy = false;
    pwm_t pwm1;
    int linear;
    int angular;
    int16_t x;       //Controll (imu or joy) raw data x
    int16_t y;       //Controll (imu or joy) raw data y
    int ticks = 0;
    bool blue = false;
    int count = 0;
    
    
    //---------------------Peripheral setup---------------------
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    pio_configuration();
    mpu = initialise_imu();
    adc = initialise_adc();
    nrf = initialise_radio();
    pwm1 = init_pwm();
    ledbuffer_t* leds = ledbuffer_init (LEDTAPE_PIO, NUM_LEDS);
    //---------------------Read configuration inputs---------------------
    
    use_joy = !pio_input_get (IMU_JOY_SEL);

    // ---------------------Setting up sleep----------------------------
    const mcu_sleep_wakeup_cfg_t sleep_wake_cfg = {  
        .pio = SLEEP_BUT_PIO,
        .active_high = false
    };
    if(!mcu_sleep_wakeup_set(&sleep_wake_cfg)) panic (LED_ERROR_PIO, INITIALISATION_ERROR);
    

    const mcu_sleep_cfg_t sleep_cfg = {  
        .mode = MCU_SLEEP_MODE_WAIT//MCU_SLEEP_MODE_SLEEP
    };

    //Flash LED to show everything initialised
    flash_led(LED_STATUS_PIO, 2);

    pacer_init (PACER_RATE); 
    uint8_t count = 0;
    while (1)
    {
        char temp_buffer[RADIO_TX_PAYLOAD_SIZE + 1];
        char tx_buffer[RADIO_TX_PAYLOAD_SIZE + 1]; // +1 for null terminator
        char rx_buffer[RADIO_RX_PAYLOAD_SIZE + 1]; // +1 for null terminator
        uint8_t rx_bytes;
        /* Wait until next clock tick.  */
        pacer_wait ();
        ticks++;
    
        // Checks if the buffer is at its end, otherwise advance the led buffer
        if (count++ == NUM_LEDS)
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
                ledbuffer_set(leds, 0, 255, 0, 255);
                ledbuffer_set(leds, NUM_LEDS / 2, 255, 0, 255);
            }
            blue = !blue;
            count = 0;
        }
        ledbuffer_write (leds);
        ledbuffer_advance (leds, 1);


        if (!pio_input_get (GPIO_JUMPER))
        {
            play_anthem(pwm1);
        }
        
        //Write to radio
        if (ticks >= 2)
        {
            // Read IMU
            task_read_imu(mpu, accel);
            // Read all ADC (joystick and battery)
            task_read_adc(adc, adc_data, sizeof(adc_data));
            //printf("%i\n",ticks);
            // Set the x-y control values based on if we are using joystick or imu
            if (use_joy)
            {
                x = adc_data[1];
                y = adc_data[2];
            }else
            {
                x = accel[0];
                y = accel[1];
            }

            //printf("ximu = %d, yimu = %d, xjoy = %d, yjoy = %d\n", accel[0], accel[1], adc_data[1], adc_data[2])
            //printf ("Bat = %d, x = %d, y = %d\n", adc_data[0], adc_data[1], adc_data[2]);

            //Convert IMU or joystick reading to scale 1-201 for x and y
            task_convert_imu_or_joy(&x, &y, &linear,&angular, use_joy);

            // Convert int values into bytes and place into tx_buffer
            tx_buffer[0] = angular & 0xFF; 
            tx_buffer[1] = linear & 0xFF;
            tx_buffer[2] = 69 & 0xFF;
            //snprintf (temp_buffer, sizeof ( temp_buffer), "Hello world %d\r\n", count++);
            //printf("angular %i linear %i 69 %i\n", tx_buffer[0], tx_buffer[1], tx_buffer[2]);
            if (! nrf24_write (nrf, tx_buffer, RADIO_TX_PAYLOAD_SIZE)) 
            {
                pio_output_set (LED_ERROR_PIO, 1);
            } else 
            {
                pio_output_set (LED_ERROR_PIO, 0);
            }

            ticks = 0;
            //pio_output_toggle (LED_ERROR_PIO);
         
        }
        
        else{
            rx_bytes = nrf24_read (nrf, rx_buffer, RADIO_RX_PAYLOAD_SIZE); // Maybe buffer needs to be 3 long same as tx...
            if ((rx_bytes != 0) && (rx_buffer[0] == 1))
            {
                int flash_times = 0;
                printf ("%i\n", rx_buffer[0]);
                pio_output_toggle (LED_STATUS_PIO);
                while(flash_times < 5)
                {
                    red_strip();
                    pacer_wait(500);
                    empty_strip();
                    pacer_wait(500);
                    flash_times++
                }
                play_card(pwm1);
                nrf24_read (nrf, rx_buffer, RADIO_RX_PAYLOAD_SIZE);
                nrf24_read (nrf, rx_buffer, RADIO_RX_PAYLOAD_SIZE);
                nrf24_read (nrf, rx_buffer, RADIO_RX_PAYLOAD_SIZE);
                nrf24_read (nrf, rx_buffer, RADIO_RX_PAYLOAD_SIZE);
                rx_buffer[0] = 0;
            }
        }

        if(low_bat_flag)
        {
            //disable LED tape...
            static int led_error_ticks = PACER_RATE/LOW_BAT_LED_RATE/2; // /2 as on for half period, off for half
            led_error_ticks--;
            if (led_error_ticks == 0)
            {
                pio_output_toggle (LED_ERROR_PIO);
                led_error_ticks = PACER_RATE/LOW_BAT_LED_RATE/2; // /2 as on for half period, off for half
            }
        } else pio_output_set (LED_ERROR_PIO, 0);
        
        // Poll sleep button and if pressed then sleep...
        if (!pio_input_get (SLEEP_BUT_PIO)) //sleep button pressed
        {
            //pio_irq_enable(WAKE_BUTTON);
            red_strip();
            play_card(pwm1);
            play_shutdown(pwm1);
            pio_output_set (LED_STATUS_PIO, 0);
            pio_output_set (LED_ERROR_PIO, 0);
            //pio_irq_enable(SLEEP_BUT_PIO);
            //irq_enable(PIO_ID(SLEEP_BUT_PIO));
            mcu_sleep(&sleep_cfg);
            flash_led(LED_STATUS_PIO, 5);
        }
        //printf("Radio channel: %d address: %d\n", nrf24_cfg.channel, nrf24_cfg.address);
    }

        
}
