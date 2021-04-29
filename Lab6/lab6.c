/********************************************************************************************
*  Lab 6  
*  Griffin Davis
*********************************************************************************************/

#include "address_map_arm.h"
#define JTAG_UART_ptr ((int*) JTAG_UART_BASE)

/*******************************************************************************
 * This program demonstrates use of the JTAG UART port
 *
 * It performs the following:
 *	1. Gets a text string from the JTAG UART
 * 	2. Extracts the digits in the string and saves them to an array
 * 	3. Echos the digit characters back to the JTAG UART
 ******************************************************************************/
/* function prototypes */


// State machine type; You may add/remove states as needed
typedef enum {
	IDLE,		
    START,		
    ENTER
} jtag_states;
 
 void put_string( char * text_string);
 char get_jtag( volatile int * ptr);
 void put_jtag( volatile int * ptr , char c );
 
int main(void) {
    
/* Declare variables and volatile pointers to I/O registers */
    jtag_states state = IDLE; 		//defines a state variable and assigns the first state
    char c;							//character read from JTAG
    // volatile int * JTAG_UART_ptr = ((int*) JTAG_UART_BASE);
    put_string("\nLab 6 by Griffin Davis.\n");
    put_string("\nPress ESC to start typing...\n");

    char digit[100];
    int i;
    for ( i=0; i<100; i++) {
        digit[i] = '\0';
    }
    i = 0;

    while (1) {
        c = get_jtag(JTAG_UART_ptr);

        switch(state){		 
            case IDLE:
                if (c == 27) {
                    state = START;
                    put_string("\nType in a string and press Enter:\n> ");
                }
                else if (c == '\0')
                    break;
                break;

            case START:
                if(  c > 0x29 && c < 0x3A ) {
                    digit[i] = c;
                    i++;
                }
                else if ( c == 10) {
                    state = ENTER;
                }
                else if (c == '\0')
                    break;
                if( c!='\0')
                   put_jtag(JTAG_UART_ptr, c);               
                break;

            case ENTER:
            	if(digit[0] != '\0') {
                	put_string(digit);
                	while (i != 0) {
                		digit[i] = '\0';
                    	i--;
                	}
                	digit[0] = '\0';
                	i = 0;
                    put_string("\n");
                }
                state = START;
                put_string("> ");
                break;      
        }
    }
}

void put_jtag(volatile int * ptr, char c) {
    int control;
    control = *(ptr +1);
    if(control & 0xFFFF0000)
        *(ptr) = c;
}

char get_jtag(volatile int * ptr) {
    int data;
    data = *(ptr);
    if(data & 0x00008000)
        return ((char)data & 0xFF);
    else
        return ('\0');
}

void put_string( char * text_string) {
    char *str;
    for (str = text_string; *str != 0; ++str)
        if(*str != '\0')
            put_jtag(JTAG_UART_ptr, *str);
}
