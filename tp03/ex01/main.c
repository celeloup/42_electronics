#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>

#define F_CPU 16000000

// Program to write 'Hello World!' via UART protocol every 2s
// Command for screen : screen /dev/ttyUSB* 115200

void uart_init(uint8_t baud)
{
    // SET BAUD RATE
    UBRR0H = (baud >> 8);  // Most significant bit
    UBRR0L = baud;         // Least significant bit

    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);    // Set frame format: 8data (Table 20-11, p.203)
    UCSR0B |= (1 << TXEN0);     // Enable  transmitter (20.11.3 p.202)
}

void uart_tx(char c)
{
    // USART Data Register Empty (UDREn) is set in USART Control and Status Register (UCSRnA)
    // = The transmit buffer (UDRn) is ready to receive data ( 20.11.2, p.200)
    while (!( UCSR0A & (1 << UDRE0)));
    UDR0 = c;   // Data Register (20.6, p.185)
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

__attribute__((signal)) void TIMER1_COMPA_vect()
{
    // PORTB ^= (1 << PB3); // DEBUG LED
    uart_printstr("Hello World!\n\r");
}

int main()
{
    // DDRB = (1 << PB3); // DEBUG LED
    
    uart_init(8); // 115200 baud rate, table 20-7 p.199

    // TIMER1 SETUP
    TCCR1B |= (1 << WGM12);     // CTC Mode (Table 16.4, p.141)
    TCCR1A |= (1 << COM1A1);
    TCCR1B |= (1 << CS12) | (1 << CS10);      // Prescaler 256 (p.143 table 16-5)
    OCR1A = 31250;    // Compare Match Value set to 2s


    // INTERRUPT SETUP
    TIMSK1 |= (1 << OCIE1A);    // Timer/Counter1 Output Compare Match A Interrupt Enable
    SREG |= (1 << 7);           // Enable global interrupt
    
    for (;;) {}
    return 0;
}