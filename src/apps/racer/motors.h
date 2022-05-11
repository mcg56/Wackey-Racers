/** @file   motors.h
    @author E. Twomey, M. Gardyne
    @date   4 May 2022
    @brief  Motor PWM Control
    @note   Needs completed
*/

#ifndef MOTORS_H
#define MOTORS_H

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "pio.h"
#include "pwm.h"
#include "panic.h"
#include "delay.h"
#include "usb_serial.h"


#define PWM_FREQ_HZ 50e3

pwm_t pwmL1;
pwm_t pwmL2;
pwm_t pwmR1;
pwm_t pwmR2;

void init_pwm (void);
void set_motor_vel (int8_t x_vel, int8_t y_vel);
void set_pwm(uint8_t motor, uint32_t vel);
void usb_to_motor (usb_serial_t *usb_serial);

#endif /* MOTORS_H */

