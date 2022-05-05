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
#include "app_radio.h"
#include "app_imu.h"
#include "hat.h"
#include "app_adc.h"
#include "app_pwm.h"
#include "app_gpio.h"
#include "adc.h"
#include "nrf24.h"
#include "spi.h"
#include "usb_serial.h"
#include "delay.h"
#include "string.h"
#include <stdio.h>

/******************************************************************************
* CONSTANTS
******************************************************************************/
#define PACER_RATE 10 //Hz

/******************************************************************************
* CONFIGURATIONS
******************************************************************************/
static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};

static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (JOYSTICK_X_ADC_CHANNEL) | BIT (JOYSTICK_Y_ADC_CHANNEL) | BIT (BATTERY_ADC_CHANNEL),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000
};
static spi_cfg_t spi_cfg =
{
    .channel = 0,
    .clock_speed_kHz = 1000,
    .cs = RADIO_CS_PIO,
    .mode = SPI_MODE_0,
    .cs_mode = SPI_CS_MODE_FRAME,
    .bits = 8
};

static nrf24_cfg_t nrf24_cfg =
{
    .channel = RADIO_CHANNEL,
    .address = RADIO_ADDRESS,
    .payload_size = RADIO_PAYLOAD_SIZE,
    .ce_pio = RADIO_CE_PIO,
    .irq_pio = RADIO_IRQ_PIO,
};




int main (void)
{
    //---------------------Variables---------------------
    twi_t mpu_twi;
    mpu_t *mpu;
    adc_t adc;
    int16_t accel[3]; // For storing imu data
    uint16_t adc_data[3]; // For storing adc data
    int radio_channel;
    bool use_joy = false;
    int linear;
    int angular;
    int16_t x;       //IMU raw data x
    int16_t y;       //IMU raw data y
    
    uint8_t count = 0; //Radio Variables
    spi_t spi;
    nrf24_t *nrf;

    //---------------------Peripheral setup---------------------
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    //Iitialise Radio 
    //Configure PIO
    pio_configuration();

    mcu_jtag_disable(); // Disable jtag so we can use TWI1

    // Initialise the TWI (I2C) bus for the MPU
    mpu_twi = twi_init (&mpu_twi_cfg);
    if (! mpu_twi) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    // Initialise the MPU9250 IMU
    mpu = mpu9250_init (mpu_twi, MPU_ADDRESS);
    if (! mpu) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    // Initialise the ADC
    adc = adc_init (&adc_cfg);
    if (! adc) panic (LED_ERROR_PIO, INITIALISATION_ERROR);
    
    // Initialise Radio
    spi = spi_init ( &spi_cfg);
    if (! spi) panic (LED_ERROR_PIO, 1);

    nrf = nrf24_init (spi, &nrf24_cfg);
    if (! nrf) panic (LED_ERROR_PIO, 2);

    //---------------------Read configuration inputs---------------------
    radio_channel = determine_radio_channel();
    printf("Radio channel: %d\n", radio_channel);

    use_joy = !pio_input_get (IMU_JOY_SEL);

    //Flash LED to show everything initialised
    flash_led(LED_STATUS_PIO, 2);

    pacer_init (PACER_RATE);
    
    while (1)
    {
        /* Wait until next clock tick.  */
        char buffer[RADIO_PAYLOAD_SIZE + 1];
        pacer_wait ();

        // Read IMU
        task_read_imu(mpu, accel);
        x = accel[0];
        y = accel[1];
        
        //Convert IMU reading to scale 0-255 for x and y
        task_convert_imu(&x, &y, &linear,&angular);
        
    
        printf("x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);
        // Linear and Angular are converted for range of 0-20 so it cant be sent in a byte
        printf("linear: %5d  angular: %5d\n", linear, angular);


        // Read all ADC
        task_read_adc(adc, adc_data, sizeof(adc_data));
        //printf ("Bat = %d, x = %d, y = %d\n", adc_data[0], adc_data[1], adc_data[2]);
  
        // Radio, need to tx and rx somehow..

        buffer[0] = linear & 0xFF; 
        buffer[1] = angular & 0xFF;
        buffer[2] = 69 & 0xFF;
        printf("%i %i %i\n", buffer[0], buffer[1], buffer[2]);

        if (! nrf24_write (nrf, buffer, RADIO_PAYLOAD_SIZE))
            pio_output_set (LED_ERROR_PIO, 1);
        else
            pio_output_set (LED_ERROR_PIO, 0);
    }
        //if recieved from car to play buzzer, then play noise

        // Led tape? not sure if this is a task or we just start it and it runs...

        // Poll sleep button and if pressed then sleep...
        
}
