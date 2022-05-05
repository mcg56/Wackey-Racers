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
#include <stdio.h>

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define PACER_RATE 10 //Hz

/******************************************************************************
* CONFIGURATIONS
******************************************************************************/
static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};

static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (JOYSTICK_X_ADC_CHANNEL) | BIT (JOYSTICK_Y_ADC_CHANNEL) | BIT (BATTERY_ADC_CHANNEL),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000
};

int main (void)
{
    //---------------------Variables---------------------
    twi_t mpu_twi;
    mpu_t *mpu;
    adc_t adc;
    int16_t accel[3]; // For storing imu data
    uint16_t adc_data[3]; // For storing adc data
    int radio_channel;
    bool use_joy = false;

    //---------------------Peripheral setup---------------------
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    //Configure PIO
    pio_configuration();

    mcu_jtag_disable(); // Disable jtag so we can use TWI1

    // Initialise the TWI (I2C) bus for the MPU
    mpu_twi = twi_init (&mpu_twi_cfg);
    if (! mpu_twi) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    // Initialise the MPU9250 IMU
    mpu = mpu9250_init (mpu_twi, MPU_ADDRESS);
    if (! mpu) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    // Initialise the ADC
    adc = adc_init (&adc_cfg);
    if (! adc) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    //---------------------Read configuration inputs---------------------
    radio_channel = determine_radio_channel();
    printf("Radio channel: %d\n", radio_channel);
    use_joy = !pio_input_get (IMU_JOY_SEL);

    //Flash LED to show everything initialised
    flash_led(LED_STATUS_PIO, 2);

    pacer_init (PACER_RATE);
    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        // Read IMU
        task_read_imu(mpu, accel);
        //printf("x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);

        // Read all ADC
        task_read_adc(adc, adc_data, sizeof(adc_data));
        //printf ("Bat = %d, x = %d, y = %d\n", adc_data[0], adc_data[1], adc_data[2]);

        // Radio, need to tx and rx somehow...
        //if recieved from car to play buzzer, then play noise

        // Led tape? not sure if this is a task or we just start it and it runs...

        // Poll sleep button and if pressed then sleep...
        
    }
}
