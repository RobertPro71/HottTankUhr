/*
 * HottTankUhr.cpp
 *
 * Created: 18.04.2020 21:47:43
 * Author : Robert
 */ 


#include <avr/io.h>
#include "hottv4.h"
#include "config.h"
#include <util/delay.h>

void SetupCpuSpeed ( void );

HoTTv4 hott;

int main(void)
{

	hott.setup();

	//Setup dig IO
	PORTB.DIRSET = PIN_LED_bm;

    while (1) 
    {
		  PORTB.OUTSET = PIN_LED_bm; //Turns ON All LEDs	
		  _delay_ms(100); //1 second delay
		  PORTB.OUTCLR = PIN_LED_bm; //Turns OFF All LEDs
		  _delay_ms(100); //1 second delay
	}
}

void SetupCpuSpeed ( void )
{
	// 20 MHz
	// Divider               19200
	//  1  =  20 MHz         = 0.2 %
	//  2  =  10 MHz         = 0.2 %
	//  4  =   5 MHz         = 1,4 %
	//  8  =   2,5 MHz       = 1.7 %
	// 16  =   1,25 MHz      = 1.7 %
	// 32  =   0,625 MHz     = 1.7 %
	// 64  =   0,3125 MHz    = 1.7 %
	//  6  =   3,3333 MHz    = 1.4 %   <-
	// 10  =   2 MHz         = 0.2 %
	// 24  =   0,8333 MHz    = 8.5 %
	// 48  =   0,4166 MHz    = 9.6 %
		
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL_MCLKCTRLA = CLKCTRL_CLKSEL_OSC20M_gc;
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL_MCLKCTRLB = CLKCTRL_PEN_bm | CLKCTRL_PDIV_6X_gc;
	
}
