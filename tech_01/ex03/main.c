#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <stdlib.h>

// Program to change RGB led color on button push (but complicated colors now)
// OC0A ROUGE
// OC0B VERT
// OC2B BLEU
# define DEBOUCE_DELAY 50000

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

char input[256] = "";
int in = 0;

void parse_cmd()
{
    // if (input[0] == "#")
    char red_s[3] = {input[1], input[2], '\0'};
    char green_s[3] = {input[3], input[4], '\0'};
    char blue_s[3] = {input[5], input[6], '\0'};
    OCR0A = (uint8_t)strtol(red_s, NULL, 16);;
    OCR0B = (uint8_t)strtol(green_s, NULL, 16);
    OCR2B = (uint8_t)strtol(blue_s, NULL, 16);
    PORTB ^= (1 << PB3);
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
    
    // LEDS
            //  R           G             B
    DDRD = (1 << PD6) | (1 << PD5) | (1 << PD3);
    DDRB = (1 << PB3);

    // TIMER 0
    TCCR0A = (1 << WGM00) | (1 << WGM01); //fast PWM mode

    // Set OC0B & OC0A on Compare Match, clear OC0B & OC0A at BOTTOM, (inverting mode).
    TCCR0A |= (1 << COM0B1) | (1 << COM0B0) | (1 << COM0A1) | (1 << COM0A0);

    // Prescaler 256
    TCCR0B |= (1 << CS02);

    OCR0A = 0xff;
    OCR0B = 0xff;

    // TIMER 2
    TCCR2A |= (1 << WGM21) | (1 << WGM20) | (1 << COM2B1) | (1 << COM2B0);
    TCCR2B |= (1 << CS22);
    OCR2B = 0xff;

    UCSR0B |= (1 << RXCIE0); // Receive complete interrupt flag
    SREG = (1 << 7);

    // BUTTON
    unsigned char last_state = PIND & (1 << PD2);

    uint8_t RED_PERCENT[3] = { 0xf2, 0x19, 0x4b};
    uint8_t GREEN_PERCENT[3] = { 0x18, 0xe0, 0x00};
    uint8_t BLUE_PERCENT[3] = { 0x4f, 0xa1, 0x82};
    uint8_t i = 0;

    for (;;)
    {
        unsigned char new_state = PIND & (1 << PD2);
        if (last_state != new_state)
        {
            if (new_state == 0)
            {
                OCR0A = RED_PERCENT[i]; //red
                OCR0B = GREEN_PERCENT[i]; //green
                OCR2B = BLUE_PERCENT[i]; //blue

                if (i < 2) {
                    i = i + 1;
                } else {
                    i = 0;
                }
            }
			for (long i = 0; i < DEBOUCE_DELAY; i++)
            {}
			last_state = new_state;
        }
    }
    return (0);
}