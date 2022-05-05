/** @file   app_imu.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to the imu peripheral
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "hat.h"
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
#include "app_init.h"
#include "adc.h"
#include "nrf24.h"
#include "spi.h"
#include "usb_serial.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"

/******************************************************************************
* FUNCTIONS
******************************************************************************/
static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};

mpu_t *initialise_imu(void)
{
    twi_t mpu_twi;
    mpu_t *mpu;
    // Disable jtag so we can use TWI1
    mcu_jtag_disable(); 
    // Redirect stdio to USB serial
    usb_serial_stdio_init ();
     // Initialise the TWI (I2C) bus for the MPU
    mpu_twi = twi_init (&mpu_twi_cfg);
    if (! mpu_twi) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    // Initialise the MPU9250 IMU
    mpu = mpu9250_init (mpu_twi, MPU_ADDRESS);
    if (! mpu) panic (LED_ERROR_PIO, INITIALISATION_ERROR);

    return mpu;
}

static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (JOYSTICK_X_ADC_CHANNEL) | BIT (JOYSTICK_Y_ADC_CHANNEL) | BIT (BATTERY_ADC_CHANNEL),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000
};

adc_t initialise_adc(void)
{
    adc_t adc;
    // Initialise the ADC
    adc = adc_init (&adc_cfg);
    if (! adc) panic (LED_ERROR_PIO, INITIALISATION_ERROR);
    return adc;
}

spi_cfg_t spi_cfg =
{
    .channel = 0,
    .clock_speed_kHz = 1000,
    .cs = RADIO_CS_PIO,
    .mode = SPI_MODE_0,
    .cs_mode = SPI_CS_MODE_FRAME,
    .bits = 8
};

nrf24_cfg_t nrf24_cfg =
{
    .channel = RADIO_CHANNEL,
    .address = RADIO_ADDRESS,
    .payload_size = RADIO_PAYLOAD_SIZE,
    .ce_pio = RADIO_CE_PIO,
    .irq_pio = RADIO_IRQ_PIO,
};

nrf24_t *initialise_radio(void)
{
    uint8_t count = 0; //Radio Variables
    spi_t spi;
    nrf24_t *nrf;
    int radio_channel;
    // Initialise Radio
    spi = spi_init ( &spi_cfg);
    if (! spi) panic (LED_ERROR_PIO, 1);

    nrf = nrf24_init (spi, &nrf24_cfg);
    if (! nrf) panic (LED_ERROR_PIO, 2);

    radio_channel = determine_radio_channel();
    printf("Radio channel: %d\n", radio_channel);

    return nrf;
}