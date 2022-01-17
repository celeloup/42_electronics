#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 16000000

// Program to ask for username and password + check and response
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

char pass[256];
char user[256];

int pass_buff = 0;
int user_buff = 0;

int pass_read = 0;
int user_read = 0;

void prompt()
{
    uart_printstr("Bonjour ! Entrez votre login:\n\r");
}

void username()
{
    uart_printstr("\tusername: ");
    user_read = 1;
}

void password()
{
    uart_printstr("\n\r\tpassword: ");
}

int done = 0;

void check()
{
    char *u = "celeloup";
    char *p = "is the best";
    if (strcmp(user, u) != 0)
    {
        // uart_printstr(user);
        // uart_printstr(pass);
        user[0] = '\0';
        user_buff = 0;
        pass[0] = '\0';
        pass_buff = 0;
        
        uart_printstr("\n\rMauvaise combinaison username/password\n\n\r");
        prompt();
        username();
        return ;
    }
    if (strcmp(pass, p) != 0)
    {
        // uart_printstr(user);
        // uart_printstr(pass);
        user[0] = '\0';
        user_buff = 0;
        pass[0] = '\0';
        pass_buff = 0;
        uart_printstr("\n\rMauvaise combinaison username/password\n\n\r");
        prompt();
        username();
        return ;
    }
    uart_printstr("\n\rCoucou ");
    uart_printstr(user);
    uart_printstr(" !");
    PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
    done = 1;
}

void USART_RX_vect()
{
    char c = UDR0;
    if (user_read)
    {
        if (c == 127 && user_buff > 0)
        {
            user[user_buff] = '\0';
            user_buff--;
            uart_printstr("\b \b");
        }
        else if (c == 13)
        {
            pass_read = 1;
            user_read = 0;
            user[user_buff] = '\0';
            password();
        }
        else if (user_buff < 255 && c != 127)
        {
            user[user_buff++] = c;
            uart_tx(c);
        }
    }
    else if (pass_read)
    {
        if (c == 127 && user_buff > 0)
        {
            pass[pass_buff] = '\0';
            pass_buff--;
            uart_printstr("\b \b");
        }
        else if (c == 13)
        {
            pass[pass_buff] = '\0';
            check();
        }
        else if (pass_buff < 255  && c != 127)
        {
            pass[pass_buff++] = c;
            uart_tx('*');
        }
    }
    if (!done)
        SREG |= (1 << 7);
}

int main()
{
    uart_init(8); // 115200 baud rate, table 20-7 p.199
    
    DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

    // INTERRUPT SETUP
    UCSR0B |= (1 << RXCIE0); // Receive complete interrupt flag
    SREG |= (1 << 7);   // Enable global interrupt

    prompt();
    username();
    
    for (;;) {}
    return 0;
}