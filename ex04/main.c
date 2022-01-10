#include <avr/io.h>

int main()
{
    // Set PB3 as output in the DDRB register
    DDRB = (1 << PB3);
    DDRD = 0x00;
    
    for(;;)
    {
        if (PIND & (1 << PIN3))
            PORTB &= ~(1 << PIN3);
        else
            PORTB |= (1 << PIN3);
    }
    return (0);
}