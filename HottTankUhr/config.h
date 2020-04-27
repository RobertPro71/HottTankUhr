/*
 * IncFile1.h
 *
 * Created: 27.04.2020 21:46:30
 *  Author: Robert
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU    3333333
#define HOTTBAUD   19200

//UART
#define PIN_TX_bp 2 // TX = PB2
#define PIN_TX_bm (1 << PIN_TX_bp)
#define PIN_RX_bp 3 // RX = PB3
#define PIN_RX_bm (1 << PIN_RX_bp)

// LED
#define PIN_LED_bp PIN2_pb
#define PIN_LED_bm PIN2_bm

// Switch
#define PIN_SWITCH_bp PIN2_pb
#define PIN_SWITCH_bm PIN2_bm

#endif /* INCFILE1_H_ */