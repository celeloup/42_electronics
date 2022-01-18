#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include <avr/io.h>

// Program to change RGB led color on button push

# define DEBOUCE_DELAY 50000
# define RED 0b1000000
# define BLUE 0b0001000
# define GREEN 0b0100000
# define YELLOW 0b1100000
# define CYAN 0b0101000
# define MAGENTA 0b1001000
# define WHITE 0b1101000

int main()
{           //  R           G             B
    DDRD = (1 << PD6) | (1 << PD5) | (1 << PD3);

    unsigned char last_state = PIND & (1 << PD2);
    uint8_t colors[7] = { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE };
    int i = 0;
    PORTD = ~colors[i];
    for (;;)
    {
        unsigned char new_state = PIND & (1 << PD2);
        if (last_state != new_state)
        {
            if (new_state == 0)
            {
                if (i < 6)
                    i++;
                else
                    i = 0;
                PORTD = ~colors[i];
            }
			for (long i = 0; i < DEBOUCE_DELAY; i++)
            {}
			last_state = new_state;
        }
    }
    return (0);
}