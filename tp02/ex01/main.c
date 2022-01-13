#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define F_CPU 16000000

// Program to toggle LED PB1 via Timer1 with duty cycle changing based
// on interruption controled by Timer0

uint8_t status = 1;
uint8_t ratio = 1;

int main()
{
// TIMER 1

    // PB1 is set to output
    DDRB = (1 << PB1);

    // configure fast PWM mode 14 (p.136)
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1A |= (1 << WGM11);
    
    // Clear OC1A on Compare Match,
    // set OC1A at BOTTOM (p.140 table 16-2)
    TCCR1A |= (1 << COM1A1);

    // Set PWM frequency/top value (1Hz frequence)
    ICR1 = F_CPU / 64 / 200;

    // Set Compare Match value -> duty cycle
    OCR1A = ratio * (ICR1 / 100);

    // Set prescaler to 64 and starts PWM
    TCCR1B |= (1 << CS11) | (1 << CS10);

// TIMER 0

    // CTC mode
    TCCR0A |= (1 << WGM01);

    // Prescaler (1024)
    TCCR0B |= (1 << CS02) | (1 << CS00);

    // Compare Match value
    OCR0A = 78;

    // Timer/Counter0 Output Compare Match A Interrupt Enable
    TIMSK0 |= (1 << OCIE0A);

    // // Enable global interrupt
    SREG |= (1 << 7);

    for(;;)
    {}
        
    return (0);
}

// Function called on interrupt
__attribute__((signal)) void TIMER0_COMPA_vect()
{
    if (ratio == 0 || ratio == 100)
        status *= -1;
    ratio += status;
    OCR1A = ratio * (ICR1 / 100);
}