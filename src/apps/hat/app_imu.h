/** @file   app_imu.h
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to the imu peripheral
*/

#ifndef APP_IMU_H
#define APP_IMU_H

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "mpu9250.h"

/******************************************************************************
* CONSTANTS
******************************************************************************/


/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
void task_read_imu(mpu_t *mpu, int16_t *accel);

#endif //APP_IMU_H
