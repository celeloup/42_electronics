#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <stdlib.h>

// Program to display value of potentiometer (ADC4)

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

void adc_init()
{
    ADCSRA = (1 << ADEN); // ADC enable (24.9.2 p.258)
    ADMUX =  (1 << REFS0) | (1 << REFS1); // Internal 1.1V Voltage Reference with external capacitor at AREF pin (24.9.1 p.s257)
    ADMUX |= (1 << MUX3); // Select pin ADC4
}

uint16_t adc_read()
{
    ADCSRA |= (1 << ADSC); // ADC Start conversion (24.9.2 p.258)
    while (ADCSRA & (1<<ADSC)); // Wait while conversion is not complete
    return ADCW; // Read data
}

int main()
{
    uart_init(8);
    adc_init();
    uint16_t val = 0;
    uint16_t old_val = val;
    char str[6] = {0};
    for (;;)
    {
        val = adc_read();
        if (val != old_val)
        {
            itoa(val * 0.08, str, 10);
            uart_printstr(str);
            uart_printstr("\n\r");
            old_val = val;
        }
    }
    return (0);
}