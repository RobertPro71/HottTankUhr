/*
 * IncFile1.h
 *
 * Created: 27.04.2020 21:46:30
 *  Author: Robert
 */ 


//  Port use
//  Port A
//    PIN 0 - Reset UPDI Debug
//    PIN 1
//    PIN 2
//    PIN 3
//    PIN 4
//    PIN 5
//    PIN 6
//    PIN 7
//  Port B
//    PIN 0
//    PIN 1
//    PIN 2 - TxD
//    PIN 3 - RxD


#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU    3333333
#define HOTTBAUD   19200

//UART
#define PIN_TX_bp PIN2_pb // TX = PB2
#define PIN_TX_bm PIN2_bm
#define PIN_RX_bp PIN3_pb // RX = PB3
#define PIN_RX_bm PIN3_bm

// LED
//#define PIN_LED_bp PIN2_pb
//#define PIN_LED_bm PIN2_bm

// Switch
//#define PIN_SWITCH_bp PIN2_pb
//#define PIN_SWITCH_bm PIN2_bm

#endif /* INCFILE1_H_ */