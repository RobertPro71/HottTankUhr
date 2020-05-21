/* 
* ad.cpp
*
* Created: 21.05.2020 13:05:29
* Author: Robert
*/


#include "ad.h"
#include <avr/io.h>

void ad::init()
{
	// VRef Configuration
	VREF.CTRLA = VREF_ADC0REFSEL_4V34_gc; // VRef == 4.34V

    // ADC Configuration
    ADC0.CTRLB = ADC_SAMPNUM_ACC1_gc; // No accumulation
    ADC0.CTRLC = ADC_REFSEL_INTREF_gc | ADC_PRESC_DIV16_gc; // Internal VRef, CLK_ADC=CLK_PER/16
    ADC0.CTRLA = ADC_RESSEL_10BIT_gc | ADC_ENABLE_bm; // 8-bit mode

//    ADC0.MUXPOS = AIN6; // select target pin

}

void ad::OnAdInterrupt()
{

}

