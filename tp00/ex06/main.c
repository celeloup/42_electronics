#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define DEBOUCE_DELAY 50000

int main()
{
    DDRB = 0xFF;
    unsigned char last_state = PIND & (1 << PD3);
    int counter = 0;

    for(;;)
    {
        unsigned char new_state = PIND & (1 << PD3);
        if (last_state != new_state)
        {
            if (new_state == 0)
            {
                if (counter < 15)
                    counter++;
                else
                    counter = 0;
                PORTB = 0;
                PORTB = counter & ((1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0));
            }
				
			for (long i = 0; i < DEBOUCE_DELAY; i++)
            {}
			last_state = new_state;
        }
    }
    return (0);
}