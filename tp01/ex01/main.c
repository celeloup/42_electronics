#include <avr/io.h>

// PROGRAM TO TOGGLE LED PB1 VIA TIMER1 WITH A 10% DUTY CYCLE (OC1A pin)
int main()
{
    // PB1 is set to output
    DDRB = (1 << PB1);

    // TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);

    // // Set to Fast PWM Mode (datasheet p.141, table 16-4)
    // TCCR1B |= (1 << WGM12);
    // TCCR1A |= (1 << WGM10);
    
    // // Toggle OC1A on Compare Match (p.140, table 16-1)
    // TCCR1A |= (1 << COM1A1); 
    
    // // Set prescaler to 256 (p.143 table 16-5) (Clock select bits)
    // TCCR1B |= (1 << CS11) | (1 << CS12);

    // // Output compare register (at which value we stop) (p.121)
    // // Frequence du CPU / prescaler / 2 -> 1Hz -> 0.5s on, 0.5s off
    // OCR1A = 180;

    OCR1A = 0x3FFF;

    TCCR1A |= ((1 << COM1A1) | (1 << COM1B1));

    // Fast PWM mode
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12)|(1 << WGM13);

    // START the timer with no prescaler
    TCCR1B |= (1 << CS10);

    for(;;)
    {}
        
    return (0);
}