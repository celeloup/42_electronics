#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define F_CPU 16000000

// Program to write 'Z' via UART protocol
// Command for screen : screen /dev/ttyUSB* 115200

void uart_tx(char c)
{
    // USART Data Register Empty (UDREn) is set in USART Control and Status Register (UCSRnA)
    // = The transmit buffer (UDRn) is ready to receive data ( 20.11.2, p.200)
    while (!( UCSR0A & (1 << UDRE0)));
    UDR0 = c;   // Data Register (20.6, p.185)
}

void uart_init(uint8_t baud)
{
    // SET BAUD RATE
    UBRR0H = (baud >> 8);  // Most significant bit
    UBRR0L = baud;         // Least significant bit

    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);    // Set frame format: 8data (Table 20-11, p.203)
    UCSR0B |= (1 << TXEN0);     // Enable  transmitter (20.11.3 p.202)
}

int main()
{
    uart_init(8); // 115200 baud rate, table 20-7 p.199
    uart_tx('Z');
}