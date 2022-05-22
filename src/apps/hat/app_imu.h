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
#define TRUE_CENTRE         600
#define IMU_ADC_LIN_MAX         (6500 + TRUE_CENTRE)
#define IMU_ADC_LIN_MIN         (-11000 + TRUE_CENTRE)

#define IMU_ADC_ANG_MAX         (15000 + TRUE_CENTRE)
#define IMU_ADC_ANG_MIN         (-15000 + TRUE_CENTRE)

#define DEADBAND_UPPER      121
#define DEADBAND_LOWER      81
#define DEADBAND_CENTRE     101
#define DEADBAND_CHANGE     20

#define LINEAR_TRANFER_MAX  201
#define LINEAR_TRANFER_MIN  1
#define NUM_ACCEL_VALUES    3

/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/
void task_read_imu(mpu_t *mpu, int16_t *accel);
void task_convert_imu_or_joy(int16_t *x, int16_t *y, int *linear, int *angular, bool use_joy);
mpu_t *initialise_imu(void);

#endif //APP_IMU_H
