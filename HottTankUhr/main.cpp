/*
 * HottTankUhr.cpp
 *
 * Created: 18.04.2020 21:47:43
 * Author : Robert
 */ 

#define LED_PIN    (1 << PORTA5)
#define BUTTON_PIN (1 << PORTA1)

#define F_CPU    8000000
#define BAUD     19200
#define BAUD_TOL 1
#define USE_2X   0

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include "hottv4.h"

void SetupCpuSpeed ( void );
void SetupRS232 ( void );

HoTTv4 hott;

uint8_t wuf;

int main(void)
{

	SetupCpuSpeed();
	hott.setup();

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

void SetupCpuSpeed ( void )
{
	CLKPSR = (1<<CLKPS0)|(1<<CLKPS1);
	CCP    = 0xd8;
	CLKPSR = 0;   // presc 1
	CLKPSR = 0;   // presc 1
	CLKPSR = 0;   // presc 1
	CLKPSR = 0;   // presc 1
}

void SetupRS232 ( void )
{
	//SETUP RS232
	UCSRB |= (1<<TXEN);                // UART TX einschalten
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	#if USE_2X
	UCSRA |= (1 << U2X);
	#else
	UCSRA &= ~(1 << U2X);
	#endif
}