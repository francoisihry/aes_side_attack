#include <stdlib.h>
# include <avr/io.h>
#include <util/delay.h>
# include <avr/interrupt.h>
#include <string.h>
#include "usart.h"
# define F_CPU 8000000
# define USART_BAUDRATE 9600
# define BAUD_PRESCALE (((( F_CPU / 16) + ( USART_BAUDRATE / 2) ) / ( USART_BAUDRATE ) ) - 1)
	/*
	This is the simplest protocol to launch AES encryption over UART.

	Each command starts with a one byte command, ASCII encoding of a letter:


	't': Test connection, return a String if the command is received.
	'p' + plaintext[16]: Send plaintext.
	'k' + key[16]: Send key.
	'g' Go, execute AES encryption
	'c' Get ciphertext, expect 16 bytes corresponding to the ciphertext.
	'f' + plaintext[16]: Fast mode, send plaintext, execute AES and give the ciphertext back. Expect 16 bytes corresponding to the ciphertext.


	At startup, a welcome message is sent by the board to the host, notifying that the boot phase has terminated.
	*/

void send_plain_text(char* input){
	// input='p' + plaintext[16]
	char plaintext[17];
	sscanf(input,"p%s",plaintext);
	plaintext[16]='\0';
	uart_puts("\n plaintext : ");
	uart_puts(plaintext);
	uart_putc('\n');
}

void send_key(char* input){
	// 'k' + key[16]
	char key[17];
	sscanf(input,"k%s",key);
	key[16]='\0';
	uart_puts("\n key : ");
	uart_puts(key);
	uart_putc('\n');
}

int main(void) {
	uart_init(USART_BAUDRATE);
	uart_puts("Welcome on the AES cipher board ! \n");
	while(1){
		char input [50]="\0";
		uart_gets(input, 50);
		char cmd=input[0];
		switch (cmd){
			case 't':
				// Test connection
				uart_putc('T');
				break;
			case 'p':
				// plaintext[16]: Send plaintext.
				uart_puts("\nSend plaintext\n");
				send_plain_text(input);
				break;
			case 'k':
				// 'k' + key[16]: Send key.
				uart_puts("\nSend key\n");
				send_key(input);
				break;
			case 'g':
				uart_puts("\nGo, execute AES encryption\n");
				break;
			case 'c':
				uart_puts("\nGet ciphertext, expect 16 bytes corresponding to the ciphertext.\n");
				break;
			case 'f':
				uart_puts("\nFast mode, send plaintext, execute AES and give the ciphertext back. Expect 16 bytes corresponding to the ciphertext.\n");
				break;
			default:
				uart_puts("\nIncorrect command\n");
		}
	}
}



