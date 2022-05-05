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

/******************************************************************************
* FUNCTIONS
******************************************************************************/
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
    // COnverts ADC Reading to a value from 0-255
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