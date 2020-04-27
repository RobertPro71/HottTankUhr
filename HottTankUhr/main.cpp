/*
 * HottTankUhr.cpp
 *
 * Created: 18.04.2020 21:47:43
 * Author : Robert
 */ 

//#define LED_PIN    (1 << PORTA5)
//#define BUTTON_PIN (1 << PORTA1)

#define F_CPU    3333333

#include <avr/io.h>
#include <util/delay.h>
//#include <util/setbaud.h>
#include "hottv4.h"

void SetupCpuSpeed ( void );
void SetupRS232 ( void );

HoTTv4 hott;

uint8_t wuf;

int main(void)
{
 
  uint32_t UBR_VAL=((F_CPU+19200 * 8)/(19200 * 16)-1);
  USART0.BAUD = (uint16_t)(UBR_VAL);
	
//	SetupCpuSpeed();
	hott.setup();

	//Setup dig IO
	PORTB.DIRSET = PIN2_bm;

	//UDR = 'x';               /* schreibt das Zeichen x auf die Schnittstelle */
	
    while (1) 
    {
		  PORTB.OUTSET = PIN2_bm; //Turns ON All LEDs	
		  _delay_ms(100); //1 second delay
		  PORTB.OUTCLR = PIN2_bm; //Turns OFF All LEDs
		  _delay_ms(100); //1 second delay
//
		//if(UCSRA & (1<<UDRE))         /* Senden, wenn UDR frei ist                    */
		//{
			//UDR = 'x';               /* schreibt das Zeichen x auf die Schnittstelle */
		}

}

void SetupCpuSpeed ( void )
{
	//CLKPSR = (1<<CLKPS0)|(1<<CLKPS1);
	//CCP    = 0xd8;
	//CLKPSR = 0;   // presc 1
	//CLKPSR = 0;   // presc 1
	//CLKPSR = 0;   // presc 1
	//CLKPSR = 0;   // presc 1


CPU_CCP = 0xD8;
CLKCTRL_MCLKCTRLA = CLKCTRL_CLKSEL_OSC20M_gc;
CPU_CCP = 0xD8;
CLKCTRL_MCLKCTRLB = 0x1;

}

void SetupRS232 ( void )
{
	////SETUP RS232
	//UCSRB |= (1<<TXEN);                // UART TX einschalten
	//UBRRH = UBRRH_VALUE;
	//UBRRL = UBRRL_VALUE;
	//#if USE_2X
	//UCSRA |= (1 << U2X);
	//#else
	//UCSRA &= ~(1 << U2X);
	//#endif
}