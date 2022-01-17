#include <avr/io.h>
void uart_init(uint8_t baud)
{
    // SET BAUD RATE
    UBRR0H = (baud >> 8);  // Most significant bit
    UBRR0L = baud;         // Least significant bit

    UCSR0C |= (1 << UCSZ01) | ( 1 << UPM01) | ( 1 << UPM00 ) | ( 1 << USBS0 );    // Set frame format: 7data Enable odd parity 2 stop bits(Table 20-11, p.203)
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);     // Enable  transmitter (20.11.3 p.202)
}

void uart_tx(char c) //send
{
    // Wait for the transmit buffer to be ready to receive data
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;   // Set buffer

}

char uart_rx(void) //read
{
    while (!(UCSR0A & (1 << RXC0)));    // Wait for data to be received
    return UDR0;    // Get and return received data from buffer
}

int MASTER = 0;
int SLAVE = 0;
int COUNTDOWN = 0;
int GAME_STARTED = 0;

void start_countdown()
{
    PORTB = ~ ( 0 );
    COUNTDOWN = 1;
}

void flash_de_la_victoire() {
    PORTB = ~ ( 0 );
}

void reset(int alert)
{
    MASTER = 0;
    SLAVE = 0;
    COUNTDOWN = 0;
    GAME_STARTED = 0;
    PORTB = 0;
    if (alert)
        uart_tx('r');
}
__attribute__((signal)) void USART_RX_vect()
{
    char c = UDR0;
    if ( c == 's' ) { // SLAVE
        SLAVE = 1;
        TIMSK1 = 0;
        start_countdown();
    }
    else if (c == 'c') { // COUNTDOWN
        PORTB = PORTB << 1;
    }
    else if (c == 'g' ) { // GAME START
        COUNTDOWN = 0;
        GAME_STARTED = 1;
    } 
    else if (c == 'w') { // WINNERRRR
        flash_de_la_victoire();
        COUNTDOWN = 0;
        GAME_STARTED = 0;
    }
    else if (c == 'l') { // LOOOSEEERRR
        GAME_STARTED = 0;
        PORTB = 0;
    }
    else if (c == 'r')
        reset(0);
}


__attribute__((signal)) void TIMER1_COMPA_vect()
{
    if (COUNTDOWN == 1)
    {
        PORTB = PORTB << 1;
        uart_tx('c');
    }
    if (PORTB == 0xF0 && MASTER && !GAME_STARTED) {
        COUNTDOWN = 0;
        GAME_STARTED = 1;
        uart_tx('g');
        TIMSK1 = 0;
    }
}

void main(void)
{
    DDRB = ~ ( 0 );//( 1 << PB3 ); //output
    DDRB |= ( 1 << PB1 ); //output
    DDRD |= (0 << PD3); //set PD3 to input mode

    uart_init(103);

    reset(1);

    // COUNTDOWN
    TCCR1B = (1 << WGM12); //CTC mode
	TCCR1B |= (1 << CS12); //256. set 0-2 bits of TCCR1B to prescalar. see: 16.11.2 & Table 16-5.
    OCR1A = F_CPU / 256 - 1; //

    // TIMSK1 |= ( 1 << OCIE1A);
    SREG |= (1 << 7);
    
    for (;;) 
    {
        if (((PIND >> PD3) & 1) == 0)
        {
            if (!MASTER && !SLAVE) //set player status
            {
                MASTER = 1;
                TIMSK1 |= ( 1 << OCIE1A);
                uart_tx('s');
                start_countdown();
            }
            else if (COUNTDOWN)
            {
                uart_tx('w');
                COUNTDOWN = 0;
                GAME_STARTED = 0;
                PORTB = 0;
            }
            else if (GAME_STARTED) {
                uart_tx('l');
                flash_de_la_victoire();
                GAME_STARTED = 0;
            }
            for (uint32_t delay = 0; delay < 800000; delay++) ;
        }
    }
}