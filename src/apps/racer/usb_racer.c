/** @file   usb_racer.c
    @author E. Twomey, M. Gardyne
    @date   11 May 2022
    @brief  USB command functions
    @note   Abstracts USB stuff from other modules
*/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "usb_racer.h"



/******************************************************************************
* USB STRUCTURES
******************************************************************************/

static usb_serial_cfg_t usb_serial_cfg =
{
    .read_timeout_us = 1,
    .write_timeout_us = 1,
};

static void
prompt_command (usb_serial_t *usb_serial)
{
    usb_serial_puts (usb_serial, "> ");
}



/******************************************************************************
* INITIALISE USB
******************************************************************************/

usb_serial_t *
init_usb (void) {
    usb_serial_t *usb_serial = usb_serial_init (&usb_serial_cfg, "");
    usb_serial_stdio_init ();

    prompt_command (usb_serial);

    return usb_serial;
}