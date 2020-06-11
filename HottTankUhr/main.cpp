/*
 * HottTankUhr.cpp
 *
 * Created: 18.04.2020 21:47:43
 * Author : Robert
 */ 


#include <avr/io.h>
#include "hottv4.h"
#include "ad.h"
#include "config.h"
#include "ahcounter_cl.h"
#include <avr/interrupt.h>
#include <util/delay.h>

void SetupCpuSpeed ( void );

HoTTv4 hott;
static HoTTv4* GetHottInterrupt;
ad AnaIn;
static ad* GetAdInterrupt;
ahcounter_cl mAh;

int main(void)
{
	GetHottInterrupt = &hott;
	GetAdInterrupt   = &AnaIn;

	//Config PIN
	PORTB.DIRSET = PIN_DEBUG_bm;
	PORTB.OUTCLR = PIN_DEBUG_bm;

	hott.setup();
	AnaIn.init();
	mAh.init();
	sei(); // Start Interrupt
	 
	while(true){
		if(AnaIn.ChannelList[VOLTAGE].newval == true){
			AnaIn.ChannelList[VOLTAGE].newval = false;
			AnaIn.ChannelList[VOLTAGE].real = (AnaIn.ChannelList[VOLTAGE].raw - AnaIn.ChannelList[VOLTAGE].offset) * AnaIn.ChannelList[VOLTAGE].scale;
		}
		if(AnaIn.ChannelList[CURRENT].newval == true){
			AnaIn.ChannelList[CURRENT].newval = false;
			AnaIn.ChannelList[CURRENT].real = (AnaIn.ChannelList[CURRENT].raw - AnaIn.ChannelList[CURRENT].offset) * AnaIn.ChannelList[CURRENT].scale;
			mAh.count(AnaIn.ChannelList[CURRENT].real);
		}
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

//Received byte complete
ISR(USART0_RXC_vect)
{
	if( GetHottInterrupt ) GetHottInterrupt->OnRcvInterrupt();
}

//Send byte complete
ISR(USART0_TXC_vect)
{
	if( GetHottInterrupt ) GetHottInterrupt->OnSndInterrupt();
}

//Timer Interrupt
ISR(TCB0_INT_vect)
{
	if( GetHottInterrupt ) GetHottInterrupt->OnUartTimerInterrupt();
}

//AD Interrupt
ISR(ADC0_RESRDY_vect)
{
	if( GetAdInterrupt ) GetAdInterrupt->OnAdInterrupt();
}

ISR(TCB1_INT_vect)
{
	if( GetAdInterrupt ) GetAdInterrupt->OnAdTimerInterrupt();
}