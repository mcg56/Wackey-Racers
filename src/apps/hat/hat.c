/** @file   hat.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Main code for the hat module

    To DO:
    -Sleep (needs to wakeup again - Angus)
    -Joystick encoded to 1-201 range (same as imu, make function general angus)
    -LED tape (anton)
    -Buzzer somehthing small on hit (anton)
    -buzzer play anthem unblocked (Plays interesting sounds - Anton)
    -Read Bat voltage and have low voltage indication (Angus)
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
#include "usb_serial.h"
#include "delay.h"
#include "string.h"
#include "mcu_sleep.h"
#include <stdio.h>

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define PACER_RATE 10 //Hz


int main (void)
{
    //---------------------Variables---------------------
    nrf24_t *nrf;
    adc_t adc;
    mpu_t *mpu;
    int16_t accel[3]; // For storing imu data
    uint16_t adc_data[3]; // For storing adc data
    bool use_joy = false;
    int linear;
    int angular;
    int16_t x;       //IMU raw data x
    int16_t y;       //IMU raw data y
    
    
    //---------------------Peripheral setup---------------------
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    pio_configuration();
    mpu = initialise_imu();
    adc = initialise_adc();
    nrf = initialise_radio();
    //---------------------Read configuration inputs---------------------
    
    use_joy = !pio_input_get (IMU_JOY_SEL);

    // Setting up sleep
    const mcu_sleep_wakeup_cfg_t sleep_wake_cfg = {  
        .pio = SLEEP_BUT_PIO,
        .active_high = false
    };
    if(!mcu_sleep_wakeup_set(&sleep_wake_cfg)) panic (LED_ERROR_PIO, INITIALISATION_ERROR);
    

    const mcu_sleep_cfg_t sleep_cfg = {  
        .mode = MCU_SLEEP_MODE_SLEEP
    };

    //Flash LED to show everything initialised
    flash_led(LED_STATUS_PIO, 2);

    pacer_init (PACER_RATE);

    while (1)
    {
        /* Wait until next clock tick.  */
        char tx_buffer[RADIO_TX_PAYLOAD_SIZE + 1]; // +1 for null terminator
        char rx_buffer[RADIO_RX_PAYLOAD_SIZE + 1]; // +1 for null terminator
        pacer_wait ();

        // Read IMU and print raw data
        task_read_imu(mpu, accel);
        //printf("x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);
        x = accel[0];
        y = accel[1];
        
        //Convert IMU reading to scale 0-255 for x and y
        task_convert_imu(&x, &y, &linear,&angular);
        
        // Linear and Angular are converted for range of 0-20 so it cant be sent in a byte
        //printf("linear: %5d  angular: %5d\n", linear, angular);

        // Read all ADC
        task_read_adc(adc, adc_data, sizeof(adc_data));
        //printf ("Bat = %d, x = %d, y = %d\n", adc_data[0], adc_data[1], adc_data[2]);
  
        // Radio, need to tx and rx somehow..
        // Convert int values into bytes and place into tx_buffer
        tx_buffer[0] = linear; 
        tx_buffer[1] = angular;
        tx_buffer[2] = 69 & 0xFF;
        
        //printf("%i %i %i\n", tx_buffer[0], tx_buffer[1], tx_buffer[2]);

        // Write to radio
        if (! nrf24_write (nrf, tx_buffer, RADIO_TX_PAYLOAD_SIZE)) pio_output_set (LED_ERROR_PIO, 1);
        else pio_output_set (LED_ERROR_PIO, 0);

        rx_bytes = nrf24_read (nrf, rx_buffer, RADIO_RX_PAYLOAD_SIZE); // Maybe buffer needs to be 3 long same as tx...
        if (rx_bytes != 0)
        {
            rx_buffer[rx_bytes] = 0;
            printf ("%s\n", rx_buffer);
            pio_output_toggle (LED_STATUS_PIO);
        }
        
        //if recieved from car to play buzzer, then play noise

        // Led tape? not sure if this is a task or we just start it and it runs...

        // Poll sleep button and if pressed then sleep...
        if (!pio_input_get (SLEEP_BUT_PIO)) //sleep button pressed
        {
            //pio_irq_enable(WAKE_BUTTON);
            pio_output_set (LED_STATUS_PIO, 0);
            pio_output_set (LED_ERROR_PIO, 0);
            mcu_sleep(&sleep_cfg);
            flash_led(LED_STATUS_PIO, 5);
        }
        printf("Radio channel: %d address: %d\n", nrf24_cfg.channel, nrf24_cfg.address);
        //printf("\n\n");
    }

        
}
