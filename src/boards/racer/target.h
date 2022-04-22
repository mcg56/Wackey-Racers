/** @file   target.h
    @author M. P. Hayes, UCECE
    @date   12 February 2018
    @brief
*/
#ifndef TARGET_H
#define TARGET_H

#include "mat91lib.h"

/* This is for the carhat (chart) board configured as a racer!  */

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

/* USB  */
//#define USB_VBUS_PIO PA5_PIO
#define USB_CURRENT_MA 500

/* LEDs  */
#define LED_ERROR_PIO PA29_PIO   //RED
#define LED_STATUS_PIO PA30_PIO  //GREEN

/* General  */
#define BATTERY_VOLATGE_PIO PA18 //Battery Voltage ADC

/* Darstedly */
#define DARSTEDLY_INPUT_PIO PB0_PIO
#define DARSTEDLY_OUTPUT_1_PIO PA20_PIO
#define DARSTEDLY_OUTPUT_2_PIO PA16_PIO

/* Button  */
#define BUTTON_PIO PA16_PIO
#define SLEEP_BUTTON_PIO PA2_PIO

/* H-bridges   */
#define MOTOR_LEFT_FORWARD_PWM_PIO PA25_PIO
#define MOTOR_LEFT_BACKWARD_PWM_PIO PA17_PIO
#define MOTOR_RIGHT_FORWARD_PWM_PIO PA0_PIO
#define MOTOR_RIGHT_BACKWARD_PWM_PIO PA1_PIO
#define H_BRIDGE_SLEEP_PIO PA15_PIO

/* Radio  */
#define RADIO_CS_PIO PA11_PIO
#define RADIO_CE_PIO PA9_PIO
#define RADIO_IRQ_PIO PA26_PIO
#define RADIO_SPCK_PIO PA14_PIO
#define RADIO_MOSI_PIO PA13_PIO
#define RADIO_MISO_PIO PA12_PIO

#define RADIO_CH1_SEL_PIO PB13_PIO
#define RADIO_CH2_SEL_PIO PB14_PIO
#define RADIO_CH3_SEL_PIO PB3_PIO
#define RADIO_CH4_SEL_PIO PB2_PIO

/* LED tape  */
#define LEDTAPE_PIO PA7_PIO

/* Bumper Switch */
#define BUMPER_SWITCH_PIO PA23_PIO //Active low

#endif /* TARGET_H  */
