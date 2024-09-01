#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "debug_prints.h"

#define BAUD0 9600 // Baud Rate for UART
#define MYUBRR (F_CPU/8/BAUD0-1) // U2X = 1

#define CE PB2
#define MOSI PB3
#define MISO PB4
#define SCK PB5

void SPI_Init();
void TC72_Select();
void TC72_DeSelect();
void Transmit_Data(uint8_t);
uint8_t Recieve_Data();


void SPI_Init()
{
	// Configure Pins
	DDRB |= (1<<CE) | (1<<MOSI) | (1<<SCK);
	DDRB &= ~(1<<MISO);
	
	// Set SPI Mode 1 (CPOL = 0, CPHA = 1)
	SPCR &= ~(1<<CPOL);
	SPCR |= (1<<CPHA);
	
	// Select Master and Enable SPI
	SPCR |= (1<<MSTR) | (1<<SPE);
	
	// Pre-scalar
	SPCR |= (1<<SPR0);	// SPI clock speed= Fosc / 16
}

void TC72_Select()
{
	PORTB |= (1<<CE);
}

void TC72_DeSelect()
{
	PORTB &= ~(1<<CE);
}

void Transmit_Data(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));	// Reading SPIF Register
}

uint8_t Recieve_Data()
{
	Transmit_Data(0x00);	// Transmit dummy data to recieve data byte
	while(!(SPSR & (1<<SPIF)));
	uint8_t recieve = SPDR;
	return recieve;
}


int main()
{
	uint8_t manufacturer_id, MSB;
	
	TC72_Select();
	
	// Initialize Interface
	SPI_Init();
	UART0_init(MYUBRR);
	
	printSerialStrln("Configured SPI...");
	printSerialStrln("Starting Conversion");
	
	// Read Manufacture ID
	TC72_Select();
	Transmit_Data(0x03);	// Send Read Command to Control Register
	manufacturer_id = Recieve_Data();
	TC72_DeSelect();
	
	// Print Manufacture ID
	printSerialStr("Manufacturer ID: ");
	printSerialInt(manufacturer_id);
	printSerialStrln("");
	
	_delay_ms(1);
	
	// Select Continuous Conversion Mode
	TC72_Select();
	Transmit_Data(0x80);	// Send Write Command
	Transmit_Data(0x00);	// Configure Continuous Mode
	TC72_DeSelect();
	
	_delay_ms(150);		// Time for first conversion
	
	while(1)
	{
		printSerialStr("Temperature: ");
		
		TC72_Select();
		Transmit_Data(0x02);	// Read MSB
		MSB = Recieve_Data();	// Receive MSB
		TC72_DeSelect();
		
		_delay_ms(1);
		
		printSerialInt(MSB);
		printSerialStrln("");
		
		_delay_ms(150);	// ADC conversion delay
	}
}