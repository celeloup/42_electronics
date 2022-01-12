#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define DEBOUCE_DELAY 50000

int main()
{
    // Set PB3 as output in the DDRB register
    DDRB = (1 << PB3);
    unsigned char last_state = PIND & (1 << PD3);

    for(;;)
    {
        unsigned char new_state = PIND & (1 << PD3);
        if (last_state != new_state)
        {
            if (new_state == 0)
				PORTB ^= (1 << PB3);
			for (long i = 0; i < DEBOUCE_DELAY; i++)
            {}
			last_state = new_state;
        }
    }
    return (0);
}