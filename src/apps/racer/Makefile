PERIPHERALS = pit pwm spi adc
DRIVERS = led pacer usb_serial panic nrf24 ledtape
TARGET = racer.bin

SRC = racer.c motors.c motors.h usb_racer.c usb_racer.h racer_pio.c racer_pio.h radio.c radio.h racer_adc.c racer_adc.h

# Use low level of optimisation to simplify debugging
OPT = -O1

BOARD = racer

include ../../boards/board.mk
