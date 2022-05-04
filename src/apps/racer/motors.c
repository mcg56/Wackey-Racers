/** @file   motors.h
    @author E. Twomey, M. Gardyne
    @date   4 May 2022
    @brief  Motor PWM Control
    @note   Needs completed
*/

#include "motors.h"

static const pwm_cfg_t pwmL1_cfg =
{
    .pio = MOTOR_LEFT_FORWARD_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwmL2_cfg =
{
    .pio = MOTOR_LEFT_BACKWARD_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwmR1_cfg =
{
    .pio = MOTOR_RIGHT_FORWARD_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwmR2_cfg =
{
    .pio = MOTOR_RIGHT_BACKWARD_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

void init_pwm (void)
{
    // Initialise PWM channels left hand and right hand sides
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    pwmL1 = pwm_init (&pwmL1_cfg);
    if (! pwm1)
        panic (LED_ERROR_PIO, 1);

    pwmL2 = pwm_init (&pwmL2_cfg);
    if (! pwm2)
        panic (LED_ERROR_PIO, 2);

    pwmR1 = pwm_init (&pwmL1_cfg);
    if (! pwm3)
        panic (LED_ERROR_PIO, 3);

    pwmR2 = pwm_init (&pwmL2_cfg);
    if (! pwm4)
        panic (LED_ERROR_PIO, 4);
    
}

void set_pwm(int motor, int vel) {
    // Set PWM for desired motor based on velocity input
    if (motor == 0) {
        if (vel > 0) {
            pwm_duty_set (pwmL1, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, vel));
            pwm_stop (pwmL2);
            pwm_start (pwmL1);   
        } else {
            pwm_duty_set (pwmL2, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, vel));
            pwm_stop (pwmL1);
            pwm_start (pwmL2); 
        }
    } else {
        if (vel > 0) {
            pwm_duty_set (pwmR1, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, vel));
            pwm_stop (pwmR2);
            pwm_start (pwmR1);   
        } else {
            pwm_duty_set (pwmR2, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, vel));
            pwm_stop (pwmR1);
            pwm_start (pwmR2);
        }
    }
}

void set_motor_vel (int x_vel, int y_vel) {
    // Set motor velocities based on recieved radio commands. 
    int left_motor_vel = 0;
    int right_motor_vel = 0;
    
    if (y_vel > 0) {

    }
}