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
#include "target.h"
#include "hat.h"
#include "pio.h"
#include "app_imu.h"
#include "mpu9250.h"

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define IMU_ADC_MAX 17000
#define IMU_ADC_MIN -17000
#define LINEAR_TRANFER_MAX 20
#define LINEAR_TRANFER_MIN 0

/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
void task_read_imu(mpu_t *mpu, int16_t *accel);
void task_convert_imu(int16_t *x, int16_t *y, int *linear, int *angular);

#endif //APP_IMU_H
