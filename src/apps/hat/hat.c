/** @file   hat.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Main code for the hat module
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "target.h"
#include "pio.h"
#include "delay.h"
#include "pacer.h"
#include "usb_serial.h"
#include "mpu9250.h"
#include "panic.h"
#include "mcu.h"

/******************************************************************************
* PRIVATE FUNCTION DEFINITIONS
******************************************************************************/
void pio_configuration(void);
void flash_led(int led_pio, int num_flash);
int determine_radio_channel(void);

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define PACER_RATE 10 //Hz


#define RADIO_CHANNEL_SEL_ERROR     1
#define INITIALISATION_ERROR        2

/******************************************************************************
* GLOBAL VARIABLES
******************************************************************************/
static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};


/******************************************************************************
* FUNCTIONS
******************************************************************************/

void pio_configuration(void)
{
    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);
    pio_config_set (FSYNC, PIO_OUTPUT_LOW);
    pio_config_set (CH1_SEL, PIO_PULLUP);
    pio_config_set (CH2_SEL, PIO_PULLUP);
    pio_config_set (CH3_SEL, PIO_PULLUP);
    pio_config_set (CH4_SEL, PIO_PULLUP);

}

void flash_led(int led_pio, int num_flash)
{
    for(int i = 0; i < num_flash; i++)
    {
        pio_output_high (led_pio);
        delay_ms (250);
        pio_output_low (led_pio);
        delay_ms (250);
    }
}

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

int determine_radio_channel(void)
{
    int radio_channel = -1;
    int num_channels_found = 0;

    if (!pio_input_get (CH1_SEL)) 
    {
        radio_channel = 1;
        num_channels_found++;
    }
    if (!pio_input_get (CH2_SEL)) 
    {
        radio_channel = 2;
        num_channels_found++;
    }
    if (!pio_input_get (CH3_SEL)) 
    {
        radio_channel = 3;
        num_channels_found++;
    }
    if (!pio_input_get (CH4_SEL)) 
    {
        radio_channel = 4;
        num_channels_found++;
    }

    if (num_channels_found > 1) //More than 1 channel select is in, only have 1 
    {
        printf("Error: Too many radio channels selected!");
        panic (LED_ERROR_PIO, RADIO_CHANNEL_SEL_ERROR);
    }
    
    if(num_channels_found == 0) //No channel selected
    {
        printf("Error: No channel selected!");
        panic (LED_ERROR_PIO, RADIO_CHANNEL_SEL_ERROR);
    }

    return radio_channel;
}

int main (void)
{
    //---------------------Variables---------------------
    twi_t mpu_twi;
    mpu_t *mpu;
    int16_t accel[3]; // For storing imu data
    int radio_channel;

    //---------------------Peripheral setup---------------------
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    //Configure PIO
    pio_configuration();

    mcu_jtag_disable(); // Disable jtag so we can use TWI1

    // Initialise the TWI (I2C) bus for the MPU
    mpu_twi = twi_init (&mpu_twi_cfg);
    //if (! mpu_twi) panic (LED_ERROR_PIO, 1);

    // Initialise the MPU9250 IMU
    mpu = mpu9250_init (mpu_twi, MPU_ADDRESS);
    if (! mpu) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    //---------------------Read inputs---------------------
    radio_channel = determine_radio_channel();
    printf("Radio channel: %d\n", radio_channel);

    //Flash LED to show everything initialised
    flash_led(LED_STATUS_PIO, 2);

    pacer_init (PACER_RATE);
    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        task_read_imu(mpu, accel);
        printf("x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);
    }
}
