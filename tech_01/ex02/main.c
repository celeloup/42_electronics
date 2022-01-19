#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include <avr/io.h>

// Program to change RGB led color on button push (but complicated colors now)
// OC0A ROUGE
// OC0B VERT
// OC2B BLEU
# define DEBOUCE_DELAY 50000

int main()
{   
    // LEDS
            //  R           G             B
    DDRD = (1 << PD6) | (1 << PD5) | (1 << PD3);
    DDRB = (1 << PB3);

    // TIMER 0
    TCCR0A = (1 << WGM00) | (1 << WGM01); //fast PWM mode

    // Set OC0B & OC0A on Compare Match, clear OC0B & OC0A at BOTTOM, (inverting mode).
    TCCR0A |= (1 << COM0B1) | (1 << COM0B0) | (1 << COM0A1) | (1 << COM0A0);

    // Prescaler 256
    TCCR0B |= (1 << CS02);

    OCR0A = 0xff;
    OCR0B = 0xff;

    // TIMER 2
    TCCR2A |= (1 << WGM21) | (1 << WGM20) | (1 << COM2B1) | (1 << COM2B0);
    TCCR2B |= (1 << CS22);
    OCR2B = 0xff;

    SREG = (1 << 7);

    // BUTTON
    unsigned char last_state = PIND & (1 << PD2);

    uint8_t RED_PERCENT[3] = { 0xf2, 0x19, 0x4b};
    uint8_t GREEN_PERCENT[3] = { 0x18, 0xe0, 0x00};
    uint8_t BLUE_PERCENT[3] = { 0x4f, 0xa1, 0x82};
    uint8_t i = 0;

    for (;;)
    {
        unsigned char new_state = PIND & (1 << PD2);
        if (last_state != new_state)
        {
            if (new_state == 0)
            {
                OCR0A = RED_PERCENT[i]; //red
                OCR0B = GREEN_PERCENT[i]; //green
                OCR2B = BLUE_PERCENT[i]; //blue

                if (i < 2) {
                    i = i + 1;
                } else {
                    i = 0;
                }
            }
            for (long i = 0; i < DEBOUCE_DELAY; i++)
            {}
            last_state = new_state;
        }
    }
    return (0);
}