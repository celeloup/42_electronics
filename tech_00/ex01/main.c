#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/eeprom.h>

// Program to display binary number on led, increment with button, persistent data

#define DEBOUCE_DELAY 50000
uint16_t magic_number = 2;
uint16_t count = 0;

int main()
{
    DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

    uint8_t is_first_run;
    is_first_run = eeprom_read_word((uint16_t *)0x02);

    if (is_first_run != magic_number)
    {
        eeprom_update_word(0x00, count);
        eeprom_update_word((uint16_t *)0x02, magic_number);
    }
    else
        count = eeprom_read_word(0x00);
    
    unsigned char last_state = PIND & (1 << PD3);
    PORTB = count;
    for (;;)
    {
        unsigned char new_state = PIND & (1 << PD3);
        if (last_state != new_state)
        {
            if (new_state == 0)
            {
                count++;
                eeprom_update_word(0x00, count);
                PORTB = count;
            }
			for (long i = 0; i < DEBOUCE_DELAY; i++)
            {}
			last_state = new_state;
        }
    }
    return (0);
}