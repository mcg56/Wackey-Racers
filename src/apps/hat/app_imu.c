/** @file   app_imu.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to the imu peripheral
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "target.h"
#include "hat.h"
#include "pio.h"
#include "app_imu.h"
#include "mpu9250.h"
#include "panic.h"

/******************************************************************************
* FUNCTIONS
******************************************************************************/
static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};

mpu_t *initialise_imu(void)
{
    twi_t mpu_twi;
    mpu_t *mpu;
    // Disable jtag so we can use TWI1
    mcu_jtag_disable(); 
     // Initialise the TWI (I2C) bus for the MPU
    mpu_twi = twi_init (&mpu_twi_cfg);
    if (! mpu_twi) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    // Initialise the MPU9250 IMU
    mpu = mpu9250_init (mpu_twi, MPU_ADDRESS);
    if (! mpu) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    return mpu;
}

void task_read_imu(mpu_t *mpu, int16_t *accel)
{
    /* Read in the accelerometer data.  */
    if (! mpu9250_is_imu_ready (mpu))
    {
        printf("Waiting for IMU to be ready...\n");
    }
    else
    {
        if (!mpu9250_read_accel (mpu, accel))
        {
            printf ("ERROR: failed to read acceleration\n");
        }
    }
}

void task_convert_imu(int16_t *x, int16_t *y, int *linear, int *angular)
{
    
    int16_t adc_range = IMU_ADC_MAX - IMU_ADC_MIN;
    int16_t linear_range = LINEAR_TRANFER_MAX - LINEAR_TRANFER_MIN;
    // COnverts ADC Reading to a value from LINEAR_TRANFER_MIN to LINEAR_TRANFER_MAX
    int linear_size = -(((*x-IMU_ADC_MIN)*linear_range)/adc_range)+LINEAR_TRANFER_MIN;
    int angular_size = -(((*y-IMU_ADC_MIN)*linear_range)/adc_range)+LINEAR_TRANFER_MIN;
    if (linear_size < LINEAR_TRANFER_MIN)
    {
        linear_size = LINEAR_TRANFER_MIN;
    }
    if (angular_size < LINEAR_TRANFER_MIN)
    {
        angular_size = LINEAR_TRANFER_MIN;
    }
    if (linear_size > LINEAR_TRANFER_MAX)
    {
        linear_size = LINEAR_TRANFER_MAX;
    }
    if (angular_size > LINEAR_TRANFER_MAX)
    {
        angular_size = LINEAR_TRANFER_MAX;
    }
    
 
    *linear = linear_size;
    *angular = angular_size;

}