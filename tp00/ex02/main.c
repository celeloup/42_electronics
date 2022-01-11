#include <avr/io.h>

int main()
{
    // Set PB3 as output in the DDRB register
    DDRB |= (1 << PB3); 
    
    // Infinite loop to keep the microchip from rebooting
    for(;;)
    {
        // Shift the PB3 pin to 1 = light up the led
        PORTB = (1 << PB3);
    }
    return (0);
}