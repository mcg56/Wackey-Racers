PERIPHERALS = pit
DRIVERS = pacer usb_serial mpu9250 panic
TARGET = hat.bin

SRC = hat.c app_imu.c app_radio.c app_gpio.c app_pwm.c app_adc.c

# Use low level of optimisation to simplify debugging
OPT = -O1

BOARD = hat

include ../../boards/board.mk #hat.h app_imu.h app_radio.h app_gpio.h app_pwm.h app_adc.h
