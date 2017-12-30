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
//uint8_t aesTestInput[16] =  {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};
uint8_t aesTestInput[16] =  {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
//AES-128 Testvector
uint8_t aes128TestKey[16] =    {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
uint8_t aes128TestOutput[16] = {0x3a,0xd7,0x7b,0xb4,0x0d,0x7a,0x36,0x60,0xa8,0x9e,0xca,0xf3,0x24,0x66,0xef,0x97};
//AES-192 Testvector
uint8_t aes192TestKey[24] =    {0x8e,0x73,0xb0,0xf7,0xda,0x0e,0x64,0x52,0xc8,0x10,0xf3,0x2b,0x80,0x90,0x79,0xe5, \
                                0x62,0xf8,0xea,0xd2,0x52,0x2c,0x6b,0x7b};
uint8_t aes192TestOutput[16] = {0xbd,0x33,0x4f,0x1d,0x6e,0x45,0xf2,0x5f,0xf7,0x12,0xa2,0x14,0x57,0x1f,0xa5,0xcc};
//AES-256 Testvector
uint8_t aes256TestKey[32] =    {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81, \
                                0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
uint8_t aes256TestOutput[16] = {0xf3,0xee,0xd1,0xbd,0xb5,0xd2,0xa0,0x3c,0x06,0x4b,0x5a,0x7e,0x3d,0xb1,0x81,0xf8};


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
	uart_puts("Welcome on the AES cipher board ! \n");
	while(1){
		// char input [50]="\0";
		// uart_gets(input, 50);
		char cmd = uart_getc();

		switch (cmd){
			case 't':
				uart_puts("\nTest connection\n");
				break;
			case 'p':
				// plaintext[16]: Send plaintext.
				uart_puts("\nSend plaintext\n");
				//send_plain_text(input);
				uart_get_data(plaintext,16);
				uart_puts("\plaintext received !\n");
				break;
			case 'k':
				// 'k' + key[16]: Send key.
				uart_puts("\nSend key\n");
				uart_get_data(key,16);
				uart_puts("\key received !\n");
				//send_key(input);
				break;
			case 'g':
				uart_puts("\nGo, execute AES encryption\n");
				break;
			case 'c':
				uart_puts("\nGet ciphertext, expect 16 bytes corresponding to the ciphertext.\n");

				uint8_t tmpOutput[16];
				uint8_t expandedKey[240];

				if(aes_expandKey (key, expandedKey, sizeof(expandedKey), AES_TYPE_128)) uart_puts("ya un blem ! \n");
							else uart_puts("ca roule chef! \n");
				aes_encryptWithExpandedKey(tmpOutput, plaintext, expandedKey, AES_TYPE_128);
					//if(memcmp(aes128TestOutput, tmpOutput, sizeof(tmpOutput)))uart_puts("ya un blem ! \n");
					//else uart_puts("ca roule chef! \n");

/*
				if(aes_expandKey (aes128TestKey, expandedKey, sizeof(expandedKey), AES_TYPE_128)) uart_puts("ya un blem ! \n");
						else uart_puts("ca roule chef! \n");
				aes_encryptWithExpandedKey(tmpOutput, aesTestInput, expandedKey, AES_TYPE_128);
				if(memcmp(aes128TestOutput, tmpOutput, sizeof(tmpOutput)))uart_puts("ya un blem ! \n");

				else uart_puts("ca roule chef! \n");*/

					uart_puts("Voici le texte chiffre \n");
					int i;
					uart_putc(' ');
					for (i = 0; i < 16; i++){
						char c[2]={'K','I'};
						sprintf(&c, "%02X", tmpOutput[i]);
						uart_puts(c);
						uart_putc(' ');

//uint8_t aes192TestOutput[16] = {0xbd,0x33,0x4f,0x1d,0x6e,0x45,0xf2,0x5f,0xf7,0x12,0xa2,0x14,0x57,0x1f,0xa5,0xcc};
					}

				break;
			case 'f':
				uart_puts("\nFast mode, send plaintext, execute AES and give the ciphertext back. Expect 16 bytes corresponding to the ciphertext.\n");
				break;
			default:
				uart_puts("\nIncorrect command\n");
		}
	}
}



