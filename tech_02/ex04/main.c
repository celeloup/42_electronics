#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <stdlib.h>
# define DEBOUCE_DELAY 50000

// Program to control RGB light with potentiometer

void adc_init()
{
    ADCSRA = (1 << ADEN); // ADC enable (24.9.2 p.258)
    ADMUX =  (1 << REFS0); // Voltage reference Selection -> ACcc (24.9.1 p.s257)
    ADMUX |= (1 << MUX2); // Select pin ADC4
    ADMUX |= (1 << ADLAR); // Left adjusted conversion for 8 bit resolution (24.9.3 p.259)
}

uint16_t adc_read()
{
    ADCSRA |= (1 << ADSC); // ADC Start conversion (24.9.2 p.258)
    while (ADCSRA & (1<<ADSC)); // Wait while conversion is not complete
    return ADCH; // Read data
}

#define RED 1
#define GREEN 2
#define BLUE 3

int main()
{
    // uart_init(8);
    adc_init();

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

    OCR0A = 0xff; // red
    OCR0B = 0xff; // green

    // TIMER 2
    TCCR2A |= (1 << WGM21) | (1 << WGM20) | (1 << COM2B1) | (1 << COM2B0);
    TCCR2B |= (1 << CS22);
    OCR2B = 0xff; // blue

    UCSR0B |= (1 << RXCIE0); // Receive complete interrupt flag
    SREG = (1 << 7);

    uint8_t val = 0;
    uint8_t old_val = val;
    char str[6] = {0};

    // BUTTON
    unsigned char last_state = PIND & (1 << PD2);

    int color = RED;

    for (;;)
    {
        val = adc_read();
        if (color == RED)
            OCR0A = val;
        else if (color == GREEN)
            OCR0B = val;
        else
            OCR2B = val;
        unsigned char new_state = PIND & (1 << PD2);
        if (last_state != new_state)
        {
            if (new_state == 0)
            {
                if (color == BLUE)
                    color = RED;
                else
                    color++;
            }
			for (long i = 0; i < DEBOUCE_DELAY; i++)
            {}
			last_state = new_state;
        }
    }
    return (0);
}