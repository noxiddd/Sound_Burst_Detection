/*
 * MyNRF24L01_RX.c
 *
 * Created: 3/19/2016 4:17:41 PM
 *  Author: Lindon
 */ 

#define F_CPU 8000000UL
#include "mirf.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define buffersize 9
uint8_t databuffer[buffersize+1];

void USART_Init( unsigned int baud );
void usart_send(uint8_t Data);
void send_string(uint8_t *Str);

int main(void)
{
	unsigned int baud = 0x33;
	DDRA = 0xFF;
		   //76543210
	DDRD = 0b00000010;
	
	//Initialize USART
	USART_Init(baud);
	
	//---TEST----
	send_string((uint8_t *)"Starting receiver ...");
	usart_send('\r');
	usart_send('\n');

	
	// Initialize AVR for use with mirf
	mirf_init();
	// Wait for mirf to come up
	_delay_ms(50);
	// Activate interrupts
	sei();
	// Configure mirf
	mirf_config();
	
	//Set address of Rx
	mirf_set_RADDR((uint8_t *)"UWI_01");

    while(1)
    {
		// Test communication
		while (!mirf_data_ready());
		mirf_get_data(databuffer);
		databuffer[buffersize]='\0';
		send_string(databuffer);
		PORTA ^= (1<<PA0);  //Toggle PA0 on reception of characters
    }
}
//-------------------------------------------------------------------------//
void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	//UBRRH = 0;
	//UBRRL = 0x33; //9600 Baud
	/* Enable receiver and transmitter */
	UCSRB = (1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}
//-------------------------------------------------------------------------//
void usart_send(uint8_t Data)
{
	while(! (UCSRA & (1<<UDRE)));
	UDR = Data;
}
//-------------------------------------------------------------------------//
void send_string(uint8_t *Str)
{
	unsigned char i;
	
	i=0;
	while(Str[i]!=0)
		usart_send(Str[i++]);
	usart_send('\r');
	usart_send('\n');		
}
