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