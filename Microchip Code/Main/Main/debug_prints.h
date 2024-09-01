/*************************************************************/
/* Function to initialize UART0 of 
 Atmega328p. 8 data bits, 1 stop bit
 and no Parity*/
void UART0_init(unsigned int ubrr);			

/* Function to read a character received by UART0.
 */
unsigned char UART0_read_char(void);		

/* Function to send a character by the UART0 at the 
pre-set baud rate
*/
void UART0_send_char(char data);
/* Function sends a C string via UART0.*/
void printSerialStr(char * array);

/* Function sends a C string via UART0 followed by a carriage return and a new line charachter. */
void printSerialStrln(char * array);

/* Function sends a floating point number coverted to string via UART0 */
void printSerialFloat(float num);

/* Function sends an integer number coverted to string via UART0 */
void printSerialInt(int num);

/*************************************************************/