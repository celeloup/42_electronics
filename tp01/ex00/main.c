#include <avr/io.h>

// PROGRAM TO TOGGLE LED PB1 VIA TIMER1 (OC1A pin)
int main()
{
    // PB1 is set to output
    DDRB = (1 << PB1);

    // Set to CTC mode (Clear Timer on Compare Match) (datasheet p.141, table 16-4)
    TCCR1B |= (1 << WGM12); 
    
    // Toggle OC1A on Compare Match (p.140, table 16-1)
    TCCR1A |= (1 << COM1A0); 
    
    // Set prescaler to 256 (p.143 table 16-5)
    TCCR1B |= (1 << CS12);

    // Output compare register (at which value we stop) (p.121)
    // Frequence du CPU / prescaler / 2 -> 1Hz -> 0.5s on, 0.5s off
    OCR1A = F_CPU / 256 / 2;

    for(;;)
    {}
        
    return (0);
}