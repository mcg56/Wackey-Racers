/** @file   motors.c
    @author E. Twomey, M. Gardyne
    @date   4 May 2022
    @brief  Motor PWM Control
    @note   Needs completing
*/

/* Take signed int from -10-10 for linear and same for angular
*/



/******************************************************************************
* INCLUDES
******************************************************************************/

#include "motors.h"



/******************************************************************************
* GLOBAL VARIABLES
******************************************************************************/

#define LINEAR_GAIN 1
#define ANGULAR_GAIN 1

typedef enum
{
    LEFT,
    RIGHT
} motor;




/******************************************************************************
* PWM STRUCTURES
******************************************************************************/

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



/******************************************************************************
* INTIALISE PWM
******************************************************************************/

void init_pwm (void)
{
    // Initialise PWM channels left hand and right hand sides
    //pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    // H-Bridge enable 
    pio_config_set (MOTOR_ENABLE_PIO, PIO_OUTPUT_HIGH);

    pwmL1 = pwm_init (&pwmL1_cfg);
    if (! pwmL1)
        panic (LED_ERROR_PIO, 1);

    pwmL2 = pwm_init (&pwmL2_cfg);
    if (! pwmL2)
        panic (LED_ERROR_PIO, 2);

    pwmR1 = pwm_init (&pwmR1_cfg);
    if (! pwmR1)
        panic (LED_ERROR_PIO, 3);

    pwmR2 = pwm_init (&pwmR2_cfg);
    if (! pwmR2)
        panic (LED_ERROR_PIO, 4);
    
}


/******************************************************************************
* SET PWM (from duty cycle)
******************************************************************************/

void set_pwm(uint8_t motor, uint32_t duty) {
    // Set PWM for desired motor based on velocity input
    switch (motor)
    {
    case LEFT:
        if (duty > 0) {
            pwm_duty_set (pwmL1, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, duty));
            pwm_stop (pwmL2);
            pwm_start (pwmL1);   
        } else {
            pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
            //pwm_duty_set (pwmL2, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, -vel));
            pwm_duty_set (pwmL2, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, duty));
            pwm_stop (pwmL1);
            pwm_start (pwmL2); 
        }
        break;
    
    case RIGHT:
        if (duty > 0) {
            pwm_duty_set (pwmR1, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, duty));
            pwm_stop (pwmR2);
            pwm_start (pwmR1);   
        } else {
            pwm_duty_set (pwmR2, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, duty));
            pwm_stop (pwmR1);
            pwm_start (pwmR2);
        }
        break;
    }
}



/******************************************************************************
* SET MOTOR VELOCITY (from x & y velocity)
******************************************************************************/

void set_motor_vel (int8_t x_vel, int8_t y_vel) {
    // Set motor velocities based on recieved radio commands. 
    uint32_t left_motor_duty = 0;
    uint32_t right_motor_duty = 0;
    
    left_motor_duty = LINEAR_GAIN*y_vel + ANGULAR_GAIN*x_vel;
    right_motor_duty = LINEAR_GAIN*y_vel - ANGULAR_GAIN*x_vel;
    set_pwm(LEFT, left_motor_duty);
    set_pwm(RIGHT, right_motor_duty);

    
}



/******************************************************************************
* USB CONTROL OF MOTORS
******************************************************************************/

void usb_to_motor (usb_serial_t *usb_serial)
{
    char buffer[80];
    char *str;
    
    str = usb_serial_gets (usb_serial, buffer, sizeof (buffer));
    if (! str)
        return;

    //usb_serial (usb_serial, "<<<%s>>>\n", str);
    //usb_serial_puts (usb_serial, str);
    //usb_serial_puts (usb_serial, str[1]);
    

    switch (str[0])
    {
    case 'f':
        usb_serial_puts (usb_serial, "Forward!\n");
        set_motor_vel (0, 70);
        break;

    case 'b':
        usb_serial_puts (usb_serial, "Backward!\n");
        set_motor_vel (0, -50);
        break;
    case 'l':
        usb_serial_puts (usb_serial, "Left!\n");
        set_motor_vel (20, 50);
        break;
        
    case 'r':
        usb_serial_puts (usb_serial, "Right!\n");
        set_motor_vel (-20, 50);
        break;

    case 's':
        usb_serial_puts (usb_serial, "Stop!\n");
        set_motor_vel (0, 0);
        break;

    default:
       break;
    }

    usb_serial_puts (usb_serial, "> ");
}