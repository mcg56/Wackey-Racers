PERIPHERALS = pit pwm
DRIVERS = led pacer usb_serial panic nrf24 
TARGET = racer.bin

SRC = racer.c motors.c motors.h usb_racer.c usb_racer.h

# Use low level of optimisation to simplify debugging
OPT = -O1

BOARD = racer

include ../../boards/board.mk
