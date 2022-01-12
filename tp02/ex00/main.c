#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#define DEBOUCE_DELAY 50000

// Program to toggle LED PB3 on button (INT1) press via Interrupt signal
int main()
{
    // PB3 is set to output
    DDRB = (1 << PB3);

    // Set External Interrupt Control Register A to generate interrupt request
    // on any logical change of INT1
    EICRA |= (1 << ISC10);

    // Set External Interrupt Mask Register
    EIMSK |= (1 << INT1);

    // Enable global interrupt
    SREG |= (1 << 7);

    for(;;)
    {}
        
    return (0);
}

// Function called on interrupt
void INT1_vect()
{
    // Switch led
    PORTB ^= (1 << PB3);
}