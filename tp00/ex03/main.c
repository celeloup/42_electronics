#include <avr/io.h>

int main()
{
    // Set PB3 as output in the DDRB register
    DDRB = (1 << PB3); 
    
    for(;;)
    {
        // For loop to act as delay
        for(unsigned long i = 0; i < 200000; i++)
        {}

        // Shift the led
        PORTB ^= (1 << PB3);
    }
    return (0);
}