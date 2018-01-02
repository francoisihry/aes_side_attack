#include <stdlib.h>
#include <stdio.h>
# include <avr/io.h>
#include <util/delay.h>
# include <avr/interrupt.h>
#include <string.h>
#include "usart.h"
#include "aes.h"

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

void uart_get_data(uint8_t *d,int size)
{
	while(size>0)
	{
	   	*d=uart_getc();
		d++;
		size--;
	}
}

void aes_128(uint8_t* plaintext, uint8_t* key, uint8_t* ciphertext){
	uint8_t tmpOutput[16];
	uint8_t expandedKey[240];
	aes_expandKey (key, expandedKey, sizeof(expandedKey), AES_TYPE_128);
	aes_encryptWithExpandedKey(ciphertext, plaintext, expandedKey, AES_TYPE_128);
}

void send_ciphertext(uint8_t* ciphertext){
	int i=0;
	for (i = 0; i < 16; i++){
		uart_putc((char)ciphertext[i]);
	}
}

int main(void) {
	uart_init(USART_BAUDRATE);

	/*
	if(aes_expandKey (aes128TestKey, expandedKey, sizeof(expandedKey), AES_TYPE_128)) uart_puts("ya un blem ! \n");
			else uart_puts("ca roule chef! \n");
	aes_encryptWithExpandedKey(tmpOutput, aesTestInput, expandedKey, AES_TYPE_128);
	if(memcmp(aes128TestOutput, tmpOutput, sizeof(tmpOutput)))uart_puts("ya un blem ! \n");
	else uart_puts("ca roule chef! \n");*/

	uint8_t plaintext[16]={};
	uint8_t key[16]={};
	uint8_t ciphertext[16]={};

	uart_puts("Welcome on the AES cipher board ! \n");
	while(1){
		// char input [50]="\0";
		// uart_gets(input, 50);
		char cmd = uart_getc();

		switch (cmd){
			case 't':
				uart_puts("OK\n");
				// 't': Test connection, return a String if the command is received.
				break;
			case 'p':
				// plaintext[16]: Send plaintext.
				//uart_puts("\nSend plaintext\n");
				//send_plain_text(input);
				uart_get_data(plaintext,16);
				//uart_puts("\plaintext received !\n");
				break;
			case 'k':
				// 'k' + key[16]: Send key.
				//uart_puts("\nSend key\n");
				uart_get_data(key,16);
				//uart_puts("\key received !\n");
				//send_key(input);
				break;
			case 'g':
				// 'g' Go, execute AES encryption
				//uart_puts("\nGo, execute AES encryption\n");
				aes_128(plaintext, key, ciphertext);
				break;
			case 'c':
				// 'c' Get ciphertext, expect 16 bytes corresponding to the ciphertext.
				send_ciphertext(ciphertext);
				break;
			case 'f':
				// uart_puts("\nFast mode, send plaintext, execute AES and give the ciphertext back. Expect 16 bytes corresponding to the ciphertext.\n");
				uart_get_data(plaintext,16);
				aes_128(plaintext, key, ciphertext);
				send_ciphertext(ciphertext);
				break;
			default:
				uart_puts("\nIncorrect command\n");
		}
	}
}



