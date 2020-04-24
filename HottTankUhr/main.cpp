/*
 * HottTankUhr.cpp
 *
 * Created: 18.04.2020 21:47:43
 * Author : Robert
 */ 

#define LED_PIN    (1 << PORTA5)
#define BUTTON_PIN (1 << PORTA1)

#define F_CPU    1000000
#define BAUD     9600
#define BAUD_TOL 1
#define USE_2X   1

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>

int main(void)
{
	//SETUP Clock speed
	//CLKPSR = (1<<CLKPS0)|(1<<CLKPS1);
	//CCP    = 0xd8;
	//CLKPSR = 0;   // presc 1
	//CLKPSR = 0;   // presc 1
	//CLKPSR = 0;   // presc 1
	//CLKPSR = 0;   // presc 1
	
	//SETUP RS232
	UCSRB |= (1<<TXEN);                // UART TX einschalten
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	#if USE_2X
		UCSRA |= (1 << U2X);
	#else
		UCSRA &= ~(1 << U2X);
	#endif

	//Setup dig IO
	DDRA = (1<<DDA5); //LED Output

	UDR = 'x';               /* schreibt das Zeichen x auf die Schnittstelle */
	
    while (1) 
    {
		PORTA |= (LED_PIN); //Turns ON All LEDs	
		_delay_ms(1000); //1 second delay
		PORTA &= ~(LED_PIN); //Turns OFF All LEDs
		_delay_ms(1000); //1 second delay

		if(UCSRA & (1<<UDRE))         /* Senden, wenn UDR frei ist                    */
		{
			UDR = 'x';               /* schreibt das Zeichen x auf die Schnittstelle */
		}

	}
}

