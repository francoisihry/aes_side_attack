#include <stdlib.h>
# include <avr/io.h>
#include <util/delay.h>
# include <avr/interrupt.h>


# define USART_BAUDRATE 9600
# define BAUD_PRESCALE (((( F_CPU / 16) + ( USART_BAUDRATE / 2) ) / ( USART_BAUDRATE ) ) - 1)
#define TRUE 1
#define FALSE 0


int main(void) {
	DDRB |= 0x01;    //Frame format.
	for (;;) {
        // Loop forever
        // switch led bleu on
        PORTB |= (1 << PB1);
        _delay_ms(1000);
        // switch led verte off
        PORTB &= ~(1 << PB1);
        _delay_ms(1000);
    }


}
