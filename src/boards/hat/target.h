/** @file   target.h
    @author M. P. Hayes, UCECE
    @date   12 February 2018
    @brief
*/
#ifndef TARGET_H
#define TARGET_H

#include "mat91lib.h"

/* This is for the carhat (chart) board configured as a hat!  */

/* System clocks  */
#define F_XTAL 12.00e6
#define MCU_PLL_MUL 16
#define MCU_PLL_DIV 1

#define MCU_USB_DIV 2
/* 192 MHz  */
#define F_PLL (F_XTAL / MCU_PLL_DIV * MCU_PLL_MUL)
/* 96 MHz  */
#define F_CPU (F_PLL / 2)

/* TWI  */
#define TWI_TIMEOUT_US_DEFAULT 10000
#define MPU_ADDRESS 0x68

/* USB  */
//#define USB_VBUS_PIO PA5_PIO
#define USB_CURRENT_MA 500
#define USB_DETECT PA5_PIO

/* ADC  */
#define BATTERY_ADC_CHANNEL    ADC_CHANNEL_1
#define JOYSTICK_X_ADC_CHANNEL ADC_CHANNEL_2
#define JOYSTICK_Y_ADC_CHANNEL ADC_CHANNEL_3


/* IMU  */
#define IMU_INT_PIO PA16_PIO

/* LEDs  */
#define LED_ERROR_PIO PA0_PIO
#define LED_STATUS_PIO PA1_PIO

/* General  */
#define APPENDAGE_PIO           PA100_PIO
#define SERVO_PWM_PIO           PA7_PIO
#define LEVEL_SHIFTER_1_3V3     PA25_PIO
#define LEVEL_SHIFTER_2_3V3     PA24_PIO
#define BUZZER_PWM              PB0_PIO
#define IMU_JOY_SEL             PB1_PIO
#define CH1_SEL                 PB14_PIO
#define CH2_SEL                 PB13_PIO
#define CH3_SEL                 PB3_PIO
#define CH4_SEL                 PB2_PIO
#define GPIO_JUMPER             PA30_PIO
#define FSYNC                   PA31_PIO
#define JOYSTICK_BUTTON         PA21_PIO



/* Button  */
#define SLEEP_BUT_PIO PA2_PIO

/* Radio  */
#define RADIO_CS_PIO PA11_PIO //SPI_CSN
#define RADIO_CE_PIO PA9_PIO
#define RADIO_IRQ_PIO PA10_PIO 

/* LED tape  */
#define LEDTAPE_PIO PA6_PIO //LED_DATA_3V3



#endif /* TARGET_H  */
