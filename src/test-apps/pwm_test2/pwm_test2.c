/* File:   pwm_test2.c
   Author: M. P. Hayes, UCECE
   Date:   15 April 2013
   Descr:  This example starts two channels simultaneously; one inverted
           with respect to the other.
*/
#include "pwm.h"
#include "pio.h"
#include "delay.h"
#include "panic.h"


#define PWM_FREQ_HZ 50e3

static const pwm_cfg_t pwm1_cfg =
{
    .pio = MOTOR_LEFT_FORWARD_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwm2_cfg =
{
    .pio = MOTOR_LEFT_BACKWARD_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwm3_cfg =
{
    .pio = MOTOR_RIGHT_FORWARD_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwm4_cfg =
{
    .pio = MOTOR_RIGHT_BACKWARD_PWM_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};


int
main (void)
{
    pwm_t pwm1;
    pwm_t pwm2;
    pwm_t pwm3;
    pwm_t pwm4;

    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (MOTOR_ENABLE_PIO, PIO_OUTPUT_HIGH);

    pwm1 = pwm_init (&pwm1_cfg);
    if (! pwm1)
        panic (LED_ERROR_PIO, 1);

    pwm2 = pwm_init (&pwm2_cfg);
    if (! pwm2)
        panic (LED_ERROR_PIO, 2);

    pwm3 = pwm_init (&pwm3_cfg);
    if (! pwm3)
        panic (LED_ERROR_PIO, 3);

    pwm4 = pwm_init (&pwm4_cfg);
    if (! pwm4)
        panic (LED_ERROR_PIO, 4);

    //pwm_channels_start (pwm_channel_mask (pwm1) | pwm_channel_mask (pwm2) | pwm_channel_mask (pwm3) | pwm_channel_mask (pwm4));
    pwm_start (pwm1);

    while (1)
    {
        delay_ms (500);
        pio_output_toggle (LED_STATUS_PIO);
    }

    return 0;
}
