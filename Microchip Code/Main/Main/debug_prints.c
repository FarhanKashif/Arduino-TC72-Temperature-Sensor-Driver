/*************************************************************/

/*************************************************************/

#include <string.h>
#include <avr/io.h>
#include <stdlib.h>
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif
#include <util/delay.h>
/* Function to initialize UART0 of 
 Atmega328p. 8 data bits, 1 stop bit
 and no Parity*/
 void UART0_init(unsigned int ubrr)
 {
    /* Set baud rate */
   UBRR0H = (unsigned char)(ubrr>>8);
   UBRR0L = (unsigned char)ubrr;
    /*set double speed operation to reduce Baudrate Error*/
    UCSR0A |= (1<<U2X0);
   /* Enable receiver and transmitter */
   UCSR0B = (1<<RXEN0)|(1<<TXEN0);
   /* Set frame format: 8data, 2stop bit */
   //UCSR0C = (1<<USBS0)|(3<<UCSZ00);
   /* Set frame format: 8data, 1stop bit */
   UCSR0C = (3<<UCSZ00); 
 }

/* Function to read a character received by UART0.
 */
unsigned char UART0_read_char(void)
{
   /* Wait for data to be received */
   while ( !(UCSR0A & (1<<RXC0)) );
   /* Get and return received data from buffer */
   return UDR0;
    
}
/* Function to send a character by the UART0 at the 
pre-set baud rate
*/
void UART0_send_char(char data)
{
   
   /* Wait for empty transmit buffer */
   while ( !( UCSR0A & (1<<UDRE0)) );
   
   /* Put data into buffer, sends the data */
   UDR0 = data;
}

void printSerialStr(char * array)
{
	char * ptr = array;
	int i = 0;

	while(*(ptr+i) != '\0')
	{
		UART0_send_char(*(ptr+i));
		i++;
	}
	
}

void printSerialStrln(char * array)
{
	char * ptr = array;
	int i = 0;

	while(*(ptr+i) != '\0')
	{
		UART0_send_char(*(ptr+i));
		i++;
	}
	UART0_send_char('\r');
	UART0_send_char('\n');
}

void printSerialFloat(float num)
{
	char buf[10];
	printSerialStr(dtostrf(num,4,2,buf));
}
void printSerialInt(int num)
{
	
	char buf[10];
	printSerialStr(itoa(num,buf,10));
	
}
void printSerialLong(long int num)
{
	
	char buf[12];
	printSerialStr(ltoa(num,buf,10));
	
}

