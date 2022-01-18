#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <string.h>
#include <avr/eeprom.h>

// Program to dump EEPROM on button push or command and write text on EEPROM on screen command

#define DEBOUCE_DELAY 50000

void uart_init(uint8_t baud)
{
    // SET BAUD RATE
    UBRR0H = (baud >> 8);  // Most significant bit
    UBRR0L = baud;         // Least significant bit

    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);    // Set frame format: 8data (Table 20-11, p.203)
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);     // Enable  transmitter (20.11.3 p.202)
}

void uart_tx(char c)
{
    // Wait for the transmit buffer to be ready to receive data
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;   // Set buffer
}

char uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)));    // Wait for data to be received
	return UDR0;    // Get and return received data from buffer
}

void uart_printstr(char *str)
{
    int i = 0;
    while (str[i])
    {
        uart_tx(str[i]);
        i++;
    }
}

void dump()
{
    for (int i = 0; i < 1024; i++)
    {
        char c = eeprom_read_byte((uint8_t *)(0x00 + i));
        
        if (c > 0 && c < 127)
            uart_tx(c);
        // uart_tx(' ');
    }
    uart_printstr("\n\r> ");
}

char input[256] = "";
int in = 0;

void parse_cmd()
{
    if (strncmp(input, "write ", 6) == 0)
    {
        for (int j = 0; j < in - 6; j++)
            eeprom_update_byte((uint8_t *)j, input[j + 6]);
        uart_printstr("> ");
    }
    else if (strncmp(input, "dump", 4) == 0)
        dump();
    else
        uart_printstr("   /!\\ Wrong command.\n\r   usage: - write [content to write]\n\r          - dump\n\r> ");
    input[0] = 0;
    in = 0;
}

__attribute__((signal)) void USART_RX_vect()
{
    char c = UDR0;
    if (c == 127 && in > 0)
    {
        input[in] = '\0';
        in--;
        uart_printstr("\b \b");
    }
    else if (c == 13)
    {
        input[in] = '\0';
        uart_tx('\n');
        uart_tx('\r');
        parse_cmd();
    }
    else if (in < 255 && c != 127)
    {
        input[in++] = c;
        uart_tx(c);
    }
}

int main()
{
    uart_init(8);
    DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

    // INTERRUPT SETUP
    UCSR0B |= (1 << RXCIE0); // Receive complete interrupt flag
    SREG |= (1 << 7);   // Enable global interrupt
    
    unsigned char last_state = PIND & (1 << PD2);
    uart_printstr("> ");
    for (;;)
    {
        unsigned char new_state = PIND & (1 << PD2);
        if (last_state != new_state)
        {
            if (new_state == 0)
            {
                uart_printstr("[dump called]\n\r");
                dump();
                PORTB ^= (1 << PB3);
            }
			for (long i = 0; i < DEBOUCE_DELAY; i++)
            {}
			last_state = new_state;
        }
    }
    return (0);
}
