/*
 * MyNRF24l01_TX.c
 *
 * Created: 3/19/2016 3:57:41 PM
 *  Author: Lindon
 */ 

#define F_CPU 8000000UL
#include "mirf.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define buffersize 9
uint8_t databuffer[]="Testing**";

int main(void)
{
	DDRA = 0xFF;
		   //76543210
	DDRD = 0b00000010;
		   	
	 // Initialize AVR for use with mirf
	 mirf_init();
	 // Wait for mirf to come up
	 _delay_ms(50);
	 // Activate interrupts
	 sei();
	 // Configure mirf
	 mirf_config();
	
	//Set receiver address
	mirf_set_TADDR((uint8_t *)"Main");
		
    while(1)
    {
		// Test communication
		mirf_send(databuffer,buffersize);
		_delay_ms(1000);
		PORTA ^= (1<<PA0);  //Toggle PA0 on transmission of characters
    }
}