#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define F_CPU 16000000

// Program to read and send back letter with inverted case, read catched by interrupt on UART port
// Command for screen : screen /dev/ttyUSB* 115200

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

// FORBIDDEN
// char uart_rx(void)
// {
// 	while (!(UCSR0A & (1 << RXC0)));    // Wait for data to be received
// 	return UDR0;    // Get and return received data from buffer
// }

__attribute__((signal)) void USART_RX_vect()
{
    PORTB ^= (1 << PB3); // DEBUG LED
    char c = UDR0;
    if (c > 90)
        uart_tx(c - 32);
    else
        uart_tx(c + 32);
}

int main()
{
    uart_init(8); // 115200 baud rate, table 20-7 p.199
    DDRB = (1 << PB3); // DEBUG LED

    // INTERRUPT SETUP
    UCSR0B |= (1 << RXCIE0); // Receive complete interrupt flag
    SREG |= (1 << 7);   // Enable global interrupt
    for (;;) {
    }
    return 0;
}