/* 
* ad.cpp
*
* Created: 21.05.2020 13:05:29
* Author: Robert
*/

#include <avr/io.h>
#include "ad.h"
#include "config.h"


#define COMPARE 33333 // 10ms
#define LOOP    10    //100ms

void ad::init()
{
	ChannelList[VOLTAGE].ad_id = AD_IN_VOLATGE;
	ChannelList[VOLTAGE].offset = 1;
	ChannelList[VOLTAGE].scale = 1.0;
	ChannelList[VOLTAGE].newval = false;
	
	ChannelList[CURRENT].ad_id = AD_IN_CURRENT;
	ChannelList[CURRENT].offset = 1;
	ChannelList[CURRENT].scale = 1.0;
	ChannelList[CURRENT].newval = false;
	
	measureChannelId = VOLTAGE;
	
	// VRef Configuration
	VREF.CTRLA = VREF_ADC0REFSEL_2V5_gc; // VRef == 2.5V

  // ADC Configuration
  ADC0.CTRLB = ADC_SAMPNUM_ACC1_gc; // No accumulation
  ADC0.CTRLC = ADC_REFSEL_INTREF_gc | ADC_PRESC_DIV16_gc; // Internal VRef, CLK_ADC=CLK_PER/16
  ADC0.CTRLA = ADC_RESSEL_10BIT_gc | ADC_ENABLE_bm; // 8-bit mode
	ADC0.INTCTRL = ADC_RESRDY_bm;
}

void ad::OnAdInterrupt()
{
	ADC0.INTFLAGS |= ADC_RESRDY_bm;
	
	if(measureChannelId == VOLTAGE){
		ChannelList[VOLTAGE].raw = ADC0.RES;
		ChannelList[VOLTAGE].newval = true;

		ADC0.MUXPOS = ChannelList[CURRENT].ad_id;
		ADC0.COMMAND = ADC_STCONV_bm;
		measureChannelId = CURRENT;
	}
	if(measureChannelId == CURRENT){
		ChannelList[CURRENT].raw = ADC0.RES;
		ChannelList[CURRENT].newval = true;
	}

}

void ad::OnAdTimerInterrupt()
{
	static uint8_t count = LOOP;

	TCB0.INTFLAGS |= TCB_CAPT_bm; //clear the interrupt flag(to reset TCB0.CNT)
	count--;
	if(count == 0){ //every 100ms
		count = LOOP;
		ADC0.MUXPOS = ChannelList[VOLTAGE].ad_id;
		ADC0.COMMAND = ADC_STCONV_bm;
		measureChannelId = VOLTAGE;	
	}
}

void ad::StartTimer()
{
    TCB1.CCMP    = COMPARE;
    TCB1.INTCTRL = TCB_CAPT_bm;	   //enable interrupt
    TCB1.CTRLA   = TCB_ENABLE_bm; //start timer   
}
