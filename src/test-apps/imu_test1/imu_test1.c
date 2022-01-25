/* File:   imu_test1.c
   Author: B Mitchell, UCECE
   Date:   15/4/2021
   Descr:  Read from an MP9250 IMU and write its output to the USB serial.
*/
#include "pio.h"
#include "delay.h"
#include <fcntl.h>
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "mpu9250.h"

static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};


static void panic (void)
{
    while(1)
    {
        pio_output_toggle (LED_ERROR_PIO);
        delay_ms (400);
    }
}


int
main (void)
{
    twi_t mpu_twi;
    mpu_t *mpu;

    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);

    // Initialise the TWI (I2C) bus for the MPU
    mpu_twi = twi_init (&mpu_twi_cfg);

    if (! mpu_twi)
        panic ();

    // Initialise the MPU9250 IMU
    mpu = mpu9250_init (mpu_twi, MPU_ADDRESS);

    if (! mpu)
        panic ();

    pacer_init (10);

    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        /* Read in the accelerometer data.  */
        if (! mpu9250_is_imu_ready (mpu))
        {
            printf("Waiting for IMU to be ready...\n");
        }
        else
        {
            int16_t accel[3];
            if (mpu9250_read_accel (mpu, accel))
            {
                printf("x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);
            }
            else
            {
                printf ("ERROR: failed to read acceleration\n");
            }
        }
    }
}
