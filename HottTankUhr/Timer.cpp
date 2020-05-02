/* 
* Timer.cpp
*
* Created: 02.05.2020 17:56:41
* Author: Robert
*/


#include "Timer.h"
#include "config.h"
#include <avr/interrupt.h>


// default constructor
Timer::Timer()
{
} //Timer

void Timer::start()
{
    TCB0.CCMP = F_CPU / 1000; //1ms
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLA = TCB_ENABLE_bm;

    sei(); //Start Interrupt
		
		PORTB.DIRSET = PIN1_bm;
		PORTB.DIRSET = PIN0_bm;
		PORTB.OUTSET = PIN0_bm;
}

volatile uint16_t count = 0;
ISR(TCB0_INT_vect)
{
	count++;
	PORTB.OUTTGL = PIN1_bm;
	//if(count == LOOP){
		//count = 0;
	//}
	TCB0.INTFLAGS |= TCB_CAPT_bm; //clear the interrupt flag(to reset TCB0.CNT)
}

