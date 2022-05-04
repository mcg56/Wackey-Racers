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

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define PACER_RATE 10 //Hz

/******************************************************************************
* GLOBAL VARIABLES
******************************************************************************/
static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};

int main (void)
{
    //---------------------Variables---------------------
    twi_t mpu_twi;
    mpu_t *mpu;
    int16_t accel[3]; // For storing imu data
    int radio_channel;

    //---------------------Peripheral setup---------------------
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    //Configure PIO
    pio_configuration();

    mcu_jtag_disable(); // Disable jtag so we can use TWI1

    // Initialise the TWI (I2C) bus for the MPU
    mpu_twi = twi_init (&mpu_twi_cfg);
    //if (! mpu_twi) panic (LED_ERROR_PIO, 1);

    // Initialise the MPU9250 IMU
    mpu = mpu9250_init (mpu_twi, MPU_ADDRESS);
    if (! mpu) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    //---------------------Read inputs---------------------
    radio_channel = determine_radio_channel();
    printf("Radio channel: %d\n", radio_channel);

    //Flash LED to show everything initialised
    flash_led(LED_STATUS_PIO, 2);

    pacer_init (PACER_RATE);
    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        task_read_imu(mpu, accel);
        printf("x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);
    }
}
