/** @file   hat.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Main code for the hat module
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

    //Flash LED to show everything initialised
    flash_led(LED_STATUS_PIO, 2);

    pacer_init (PACER_RATE);

    while (1)
    {
        /* Wait until next clock tick.  */
        char buffer[RADIO_PAYLOAD_SIZE + 1];
        pacer_wait ();

        // Read IMU and print raw data
        task_read_imu(mpu, accel);
        printf("x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);
        x = accel[0];
        y = accel[1];
        
        //Convert IMU reading to scale 0-255 for x and y
        task_convert_imu(&x, &y, &linear,&angular);
        
        // Linear and Angular are converted for range of 0-20 so it cant be sent in a byte
        printf("linear: %5d  angular: %5d\n", linear, angular);

        // Read all ADC
        task_read_adc(adc, adc_data, sizeof(adc_data));
        //printf ("Bat = %d, x = %d, y = %d\n", adc_data[0], adc_data[1], adc_data[2]);
  
        // Radio, need to tx and rx somehow..
        // Convert int values into bytes and place into buffer
        buffer[0] = linear & 0xFF; 
        buffer[1] = angular & 0xFF;
        buffer[2] = 69 & 0xFF;
        
        printf("%i %i %i\n", buffer[0], buffer[1], buffer[2]);

        // Write to radio
        if (! nrf24_write (nrf, buffer, RADIO_PAYLOAD_SIZE)) pio_output_set (LED_ERROR_PIO, 1);
        else pio_output_set (LED_ERROR_PIO, 0);
        
        //if recieved from car to play buzzer, then play noise

        // Led tape? not sure if this is a task or we just start it and it runs...

        // Poll sleep button and if pressed then sleep...
        printf("\n\n");
    }

        
}
