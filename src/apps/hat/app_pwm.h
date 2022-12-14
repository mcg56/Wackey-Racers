/** @file   app_pwm.h
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to pwms (mainly buzzer)
*/

#ifndef APP_PWM_H
#define APP_PWM_H

#include "pwm.h"
/******************************************************************************
* CONSTANTS
******************************************************************************/
#define PWM_FREQ_HZ 1000
#define SERVO_PWM_FREQ_HZ 50
#define PWM_DUTY 50
#define SERVO_PWM_DUTY 50
#define NUM_LEDS 23

/******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
******************************************************************************/

pwm_t init_pwm (void);
pwm_t init_pwm2 (void);
void play_card (pwm_t pwm1);
void play_anthem (pwm_t pwm1);
void play_shutdown (pwm_t pwm1);
void spin_flags(pwm_t pwm2);

#endif //APP_PWM_H