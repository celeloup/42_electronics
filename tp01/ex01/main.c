#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define F_CPU 16000000

// PROGRAM TO TOGGLE LED PB1 VIA TIMER1 WITH A 10% DUTY CYCLE (OC1A pin)
int main()
{
    // PB1 is set to output
    DDRB = (1 << PB1);

    // configure fast PWM mode 14 (p.136)
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1A |= (1 << WGM11);
    
    // Clear OC1A on Compare Match,
    // set OC1A at BOTTOM (p.140 table 16-2)
    TCCR1A |= (1 << COM1A1);

    // Set PWM frequency/top value (1Hz frequence)
    ICR1 = F_CPU / 256;

    // Set Compare Match value -> duty cycle
    OCR1A = ICR1 / 10;

    // Set prescaler to 64 and starts PWM
    TCCR1B |= (1 << CS12);

    for(;;)
    {}

    return (0);
}