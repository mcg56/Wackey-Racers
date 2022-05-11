/* File:   pwm_test2.c
   Author: M. P. Hayes, UCECE
   Date:   15 April 2013
   Descr:  This example starts two channels simultaneously; one inverted
           with respect to the other.
*/
#include "pwm.h"
#include "pio.h"
#include "delay.h"
#include "panic.h"
#include "notes.h"
#include "delay.h"

#define PWM1_PIO BUZZER_PWM

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

int card_melody[] = {
  NOTE_C6, NOTE_E5, NOTE_DS5, NOTE_D5, NOTE_C5
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
int card_note_Durations[] = {
  6, 5, 5, 5, 1
};


#define PWM_FREQ_HZ 1000
#define PWM_DUTY 50

static const pwm_cfg_t pwm1_cfg =
{
    .pio = PWM1_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, PWM_DUTY),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};


int
main (void)
{
    pwm_t pwm1;
 
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);

    pwm1 = pwm_init (&pwm1_cfg);
    if (! pwm1)
        panic (LED_ERROR_PIO, 1);

    pwm_channels_start (pwm_channel_mask (pwm1));

    int thisNote = 0;
    int count = 0;
    while (1)
    {
        thisNote = 0;

        for (thisNote; thisNote < sizeof(card_melody)/sizeof(int); thisNote++) 
        {
            //Play note
            int noteDuration = 800 / card_note_Durations[thisNote];
            pwm_frequency_set (pwm1, card_melody[thisNote]);
            pwm_channels_start (pwm_channel_mask (pwm1));
            delay_ms (noteDuration);

            //Delay between note
            int pauseBetweenNotes = noteDuration * 1.05;
            pwm_channels_stop(pwm_channel_mask (pwm1));
            delay_ms(pauseBetweenNotes);
        }
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
        }

        count++;

        if (count == 1)
        {
            pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);
        } else{
            pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);
            count = 0;
        }

    }

    return 0;
}

