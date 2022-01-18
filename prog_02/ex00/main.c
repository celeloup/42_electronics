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
    // on any logical change of INT0 (button on pin PD2 on board week 2)
    EICRA |= (1 << ISC00);

    // Set External Interrupt Mask Register
    EIMSK |= (1 << INT0);

    // Enable global interrupt
    SREG |= (1 << 7);


    for(;;)
    {}

    return (0);
}

int input = 0;

// Function called on interrupt
__attribute__((signal)) void INT0_vect()
{
    if (input == 0)
    {
        PORTB ^= (1 << PB3);
        input = 1;
    }
    else
        input = 0;
    for (long i = 0; i < DEBOUCE_DELAY; i++)
    {}
}
