#include <avr/io.h>

int main()
{
    // Set PB3 as output in the DDRB register
    DDRB = (1 << PB3);
    
    for(;;)
    {
        if (PIND & (1 << PD3))
            PORTB = (0 << PB3);
        else
            PORTB = (1 << PB3);
    }
    return (0);
}