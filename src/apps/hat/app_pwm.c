/** @file   app_pwm.c
    @author A. Musalov, A. Eason
    @date   4/05/2022
    @brief  Functions and structs relating to pwms (mainly buzzer)
*/

/******************************************************************************
* INCLUDES
******************************************************************************/
#include "target.h"
#include "hat.h"
#include "pio.h"
#include "notes.h"
#include "pwm.h"
#include "delay.h"
#include "app_pwm.h"
#include "panic.h"
#include "app_gpio.h"



static const pwm_cfg_t pwm1_cfg =
{
    .pio = BUZZER_PWM,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, PWM_DUTY),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};


pwm_t init_pwm (void)
{
    pwm_t pwm1;
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);

    pwm1 = pwm_init (&pwm1_cfg);
    if (! pwm1)
        panic (LED_ERROR_PIO, 1);

    
    //pwm_channels_start (pwm_channel_mask (pwm1));

    return pwm1;
}

void play_card (pwm_t pwm1)
{
    int card_melody[] = {
    NOTE_C6, NOTE_E5, NOTE_DS5, NOTE_D5, NOTE_C5
    };

    int card_note_Durations[] = {
    3, 5, 5, 5, 1
    };

    int thisNote = 0;
    int count = 0;
    thisNote = 0;

    for (thisNote; thisNote < sizeof(card_melody)/sizeof(int); thisNote++) 
    {
        //Play note
        int noteDuration = 700 / card_note_Durations[thisNote];
        pwm_frequency_set (pwm1, card_melody[thisNote]);
        pwm_channels_start (pwm_channel_mask (pwm1));
        red_strip();
        delay_ms (noteDuration);

        //Delay between note
        int pauseBetweenNotes = noteDuration * 0.9;
        pwm_channels_stop(pwm_channel_mask (pwm1));
        empty_strip();
        delay_ms(pauseBetweenNotes);
    }
    
}

void play_shutdown (pwm_t pwm1)
{
    int shutdown_melody[] = {
    NOTE_GS5, NOTE_DS5, NOTE_GS4, NOTE_AS4
    };

    int shutdown_note_Durations[] = {
    5, 5, 5, 5
    };

    int thisNote = 0;
    int count = 0;
    thisNote = 0;
    
    for (thisNote; thisNote < sizeof(shutdown_melody)/sizeof(int); thisNote++) 
    {
        //Play note
        int noteDuration = 1400 / shutdown_note_Durations[thisNote];
        pwm_frequency_set (pwm1, shutdown_melody[thisNote]);
        pwm_channels_start (pwm_channel_mask (pwm1));
        delay_ms (noteDuration);

        //Delay between note
        int pauseBetweenNotes = noteDuration * 0.8;
        pwm_channels_stop(pwm_channel_mask (pwm1));
        delay_ms(pauseBetweenNotes);
        
    }
    
    
}

void play_anthem(pwm_t pwm1)
{
    int anthem_melody[] = {
    NOTE_G4, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, 
    NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_C4, 
    NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, 
    NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_G4, 
    NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, 
    NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_C4, NOTE_C4, 
    NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_B4, NOTE_C5, NOTE_D5, 
    NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_G4, NOTE_B4, NOTE_C5, NOTE_D5,
    NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_G4, NOTE_A4, NOTE_B4,
    NOTE_C5, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_F5,
    NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
    NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_A4,
    NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_C4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
    };

    int anthem_note_Durations[] = {
    8, 4, 6, 16, 4, 8, 8, 
    4, 6, 16, 4, 8, 8, 
    4, 8, 8, 4, 8, 8, 4, 8, 8, 2,
    4, 6, 16, 4, 8, 8, 
    4, 6, 16, 4, 8, 8, 
    4, 6, 16, 4, 6, 16, 
    4, 6, 16, 8, 8, 8, 8, 
    2, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8,
    2, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8,
    4, 6, 16, 4, 6, 16, 4, 8, 8, 2,
    2, 8, 8, 8, 8, 3, 8, 2,
    2, 8, 8, 8, 8, 3, 8, 2,
    4, 6, 16, 4, 4, 2, 4, 4, 1
    };

    int thisNote = 0;
    int count = 0;
    while (!pio_input_get (GPIO_JUMPER))
    {
        thisNote = 0;
        
        for (thisNote; thisNote < sizeof(anthem_melody)/sizeof(int); thisNote++) 
        {
            //Play note
            int noteDuration = 2300 / anthem_note_Durations[thisNote];
            pwm_frequency_set (pwm1, anthem_melody[thisNote]);
            pwm_channels_start (pwm_channel_mask (pwm1));
            delay_ms (noteDuration);

            //Delay between note
            int pauseBetweenNotes = noteDuration * 1.05;
            pwm_channels_stop(pwm_channel_mask (pwm1));
            delay_ms(pauseBetweenNotes);
            if (pio_input_get (GPIO_JUMPER))
            {
                break;
            }

        }
    }
}