//----------------------------------------------------------------------
// HottV4
//
// Implementation of HoTT V4 protocol
//
// Author: Heinz Bruederlin
//----------------------------------------------------------------------
#include "hottv4.h"
#include "config.h"
#include <string.h>

//static void itoa8(char* str, uint8_t i, uint8_t digits) {
    //int j;
    //uint8_t div = 100;
    //for (j=2; j>=0; j--) {
	//*str++ = i/div + '0';
	//i -= i/div*div;
	//div /= 10;
	//if (digits && j==digits) *str++ = '.';
    //}
    //*str++ = '\0';
//}

//static void itoa16(char* str, uint16_t i, uint8_t digits) {
    //int j;
    //uint16_t div = 10000;
    //for (j=4; j>=0; j--) {
	//*str++ = i/div + '0';
	//i -= i/div*div;
	//div /= 10;
	//if (digits && j==digits) *str++ = '.';
    //}
    //*str++ = '\0';
//}

//static void itoa32(char* str, uint32_t i, uint8_t digits) {
    //int j;
    //uint32_t div = 1000000000;
    //for (j=9; j>=0; j--) {
	//*str++ = i/div + '0';
	//i -= i/div*div;
	//div /= 10;
	//if (digits && j==digits) *str++ = '.';
    //}
    //*str++ = '\0';
//}

HoTTv4::HoTTv4(){
}

void HoTTv4::setup(){
    gam.startByte      = HOTTV4_START_BIN;
    gam.sensorID       = HOTTV4_GENERAL_AIR_SENSOR_ID;
    gam.sensorTextID   = HOTTV4_GENERAL_AIR_SENSOR_TEXT_ID;
    gam.endByte        = HOTTV4_STOP;
    gam.lowestCellVoltage = 0;
    gam.lowestCellNumber  = 0;

    txt.startByte      = HOTTV4_START_TXT;
    txt.sensorTextID   = HOTTV4_ELECTRICAL_AIR_SENSOR_TEXT_ID;
    txt.endByte        = HOTTV4_STOP;

    txtClear();

	UartInit();	
    //enableRx();	
}

void HoTTv4::UartInit(){
	PORTB.DIRSET = PIN_TX_bm;
	PORTB.OUTSET = PIN_TX_bm;

	uint32_t UBR_VAL = ((F_CPU * 64)/(HOTTBAUD * 16));
	USART0.BAUD = (uint16_t)(UBR_VAL);	
	
	USART0.CTRLC =
		USART_CMODE_ASYNCHRONOUS_gc | // Mode: Asynchronous[default]
		USART_PMODE_DISABLED_gc | // Parity: None[default]
		USART_SBMODE_1BIT_gc | // StopBit: 1bit[default]
		USART_CHSIZE_8BIT_gc; // CharacterSize: 8bit[default]

	//USART0.CTRLA =
		//USART_RXCIE_bm | // Enable RX interrupt
		//!USART_TXCIE_bm; // Disable TX interrupt

	USART0.CTRLB =
		USART_RXEN_bm | // Start Receiver
		USART_TXEN_bm | // Start Transmitter
		USART_RXMODE_NORMAL_gc; // Receiver mode is Normal USART & 1x-speed

}

void HoTTv4::UartEnableRx() {
	USART0.CTRLB = USART_RXEN_bm; 
}

void HoTTv4::UartEnableTx() {
	USART0.CTRLB = USART_TXEN_bm;
}

//void HoTTv4::write(uint8_t c) {
  //ss.write(c);
//}
//
//void HoTTv4::sendData(uint8_t *data, uint8_t size) {
    //ss.flush();
//
    //delay(5); // Protocol specific waiting time to avoid collisions
//
    //if (ss.available() > 0) return;
//
    //enableTx();
//
    //uint16_t crc = 0;
    //for (uint8_t i = 0; i < size; i++) {
	//uint8_t d = *data++;
	//crc += d;
	//write(d);
//
	//// Protocol specific delay between each transmitted byte
	//delayMicroseconds(1000);
    //}
//
    //write(crc & 0xFF); // Write package checksum
    //// Protocol specific delay between each transmitted byte
    //delayMicroseconds(1000);
    //ss.flush();
//
    //enableRx();
//}
//
//
//static const char uicMenu0[]  PROGMEM = " UIC CONFIG";
//static const char uicMenu1[]  PROGMEM = " MIN VOLT ";
//static const char uicMenu2[]  PROGMEM = " MAX CURR ";
//static const char uicMenu3[]  PROGMEM = " MIN CAP  ";
//static const char uicMenu4[]  PROGMEM = " CAL VOLT ";
//static const char uicMenu5[]  PROGMEM = " DBG      ";
//static const char menuOn[]    PROGMEM = "on";
//static const char menuOff[]   PROGMEM = "off";
//static const char menuStore[] PROGMEM = "sto";
//static const char menuArrow[] PROGMEM = ">";
//#define menuBool(a) ((a)?menuOn:menuOff)
//
//void HoTTv4::uicText(uint8_t c, uint8_t sensorTextID) {
    //char buf[7];
    //uint8_t x;
//
    //txt.sensorTextID = sensorTextID;
//
    //switch(c & 0x0F) {
	//case HOTTV4_TEXT_MODE_LEFT:
	    //if (col==1 || col ==2) {
		//col = 0;
	    //}
	    //break;
	//case HOTTV4_TEXT_MODE_RIGHT:
	//case HOTTV4_TEXT_MODE_UP:
	    //if (col==0) {
		//if (row > 1) row--;
	    //} else if (col==1) {
		//uint8_t speedup = ((c&0xF)==HOTTV4_TEXT_MODE_RIGHT) ? 10 : 1;
		//switch(row) {
		    //case 1: if (cfg->minVoltage < 550)
				//cfg->minVoltage  += (1 * speedup);
			    //break;
		    //case 2: if (cfg->maxCurrent < 100)
				//cfg->maxCurrent  += (1 * speedup);
			    //break;
		    //case 3: if (cfg->minCapacity < 10000)
				//cfg->minCapacity += (10 * speedup);
			    //break;
		    //case 4: if (cfg->calVoltage < 550)
				//cfg->calVoltage  += (1 * speedup);
			    //break;
		//}
	    //} else {
		//switch(row) {
		    //case 1: cfg->alarmVoltage  = true;
			    //break;
		    //case 2: cfg->alarmCurrent  = true;
			    //break;
		    //case 3: cfg->alarmCapacity = true;
			    //break;
		//}
	    //}
	    //break;
	//case HOTTV4_TEXT_MODE_DOWN:
	    //if (col==0) {
		//if (row < 5) row++;
	    //} else if (col==1) {
		//switch(row) {
		    //case 1: if (cfg->minVoltage  > 0) cfg->minVoltage  -= 1;
			    //break;
		    //case 2: if (cfg->maxCurrent  > 0) cfg->maxCurrent  -= 1;
			    //break;
		    //case 3: if (cfg->minCapacity > 0) cfg->minCapacity -= 10;
			    //break;
		    //case 4: if (cfg->calVoltage  > 0) cfg->calVoltage  -= 1;
			    //break;
		//}
	    //} else {
		//switch(row) {
		    //case 1: cfg->alarmVoltage  = false;
			    //break;
		    //case 2: cfg->alarmCurrent  = false;
			    //break;
		    //case 3: cfg->alarmCapacity = false;
			    //break;
		//}
	    //}
	    //break;
	//case HOTTV4_TEXT_MODE_SET:
            //if (row==5) { dbgText(); return; }
	    //col++;
	    //if (col == 3) {
		//col = 0;
		//if (row == 4) {
		    //uint16_t adc = analogRead(VBAT);
		    //cfg->calcCalibration(cfg->calVoltage, adc);
		//}
		//cfg->write();
	    //}
	    //break;
	//case HOTTV4_TEXT_MODE_NIL:
	    //break;
    //}
//
    //txtPrint(0, 0,21, uicMenu0, true, false);
    //txtPrint(1, 0,10, uicMenu1, true, false);
    //txtPrint(2, 0,10, uicMenu2, true, false);
    //txtPrint(3, 0,10, uicMenu3, true, false);
    //txtPrint(4, 0,10, uicMenu4, true, false);
    //txtPrint(5, 0,10, uicMenu5, true, false);
//
    //itoa16(buf, cfg->Version,2);
    //txtPrint(0,14,6, buf, false, false);
//
    //itoa16(buf, cfg->minVoltage, 1);
    //txtPrint(1,10,7, buf, false, false);
    //itoa16(buf, cfg->maxCurrent, 0);
    //txtPrint(2,10,7, buf, false, false);
    //itoa16(buf, cfg->minCapacity,0);
    //txtPrint(3,10,7, buf, false, false);
    //itoa16(buf, cfg->calVoltage, 1);
    //txtPrint(4,10,7, buf, false, false);
//
    //txtPrint(1,17,5, menuBool(cfg->alarmVoltage),  true, false);
    //txtPrint(2,17,5, menuBool(cfg->alarmCurrent),  true, false);
    //txtPrint(3,17,5, menuBool(cfg->alarmCapacity), true, false);
    //txtPrint(4,17,5, menuStore,                    true, false);
//
    //if      (col==0) x = 0;
    //else if (col==1) x = 9;
    //else if (col==2) x = 16;
    //txtPrint(row, x, 1, menuArrow, true, false);
//}
//
//
//////----------------------- General Module (GAM) -----------------
//void HoTTv4::gamSetCellVoltage(uint8_t no, uint8_t volt) {
    //uint8_t* cell;
    //if (no<1 || no>6) return;
    //cell = &gam.voltageCell1+no-1;
    //*cell = volt;
    //if (gam.lowestCellVoltage==0 || volt < gam.lowestCellVoltage) {
        //gam.lowestCellVoltage = volt;
        //gam.lowestCellNumber  = no;
    //}
//}
//
//void HoTTv4::gamSetBattery1(uint16_t volt) {
    //gam.battery1 = volt;
//}
//
//void HoTTv4::gamSetBattery2(uint16_t volt) {
    //gam.battery2 = volt;
//}
//
//void HoTTv4::gamSetTemperature1(uint8_t temp) {
    //gam.temperature1 = temp + GAM_TEMP_OFFSET;
//}
//
//void HoTTv4::gamSetTemperature2(uint8_t temp) {
    //gam.temperature2 = temp + GAM_TEMP_OFFSET;
//}
//
//void HoTTv4::gamSetFuelPercent(uint8_t percent) {
    //gam.fuelPercent = percent;
//}
//
//void HoTTv4::gamSetFuelCapacity(int16_t capacity) {
    //gam.fuelCapacity = capacity;
//}
//
//void HoTTv4::gamSetRPM(uint16_t rpm) {
    //gam.rpm = rpm;
//}
//
//void HoTTv4::gamSetRPM2(uint16_t rpm) {
    //gam.rpm2 = rpm;
//}
//
//void HoTTv4::gamSetAltitude(uint16_t altitude) {
    //uint32_t now = millis();
    //gam.altitude = altitude - ALT_OFFSET;
    //if (now > gam_m1s) {
	//gam.m1s = altitude - gam_m1;
	//gam_m1  = altitude;
	//gam_m1s += 1000;
    //}
    //if (now > gam_m3s) {
	//gam.m3s = altitude - gam_m3;
	//gam_m3  = altitude;
	//gam_m3s += 3000;
    //}
//}
//
//void HoTTv4::gamSetCurrent(uint16_t current) {
    //gam.current = current;
    //if (cfg->alarmCurrent && gam.current > cfg->maxCurrent) {
	//gam.inverseStatus2 |=   1<<0; /* current invers */
    //} else {
	//gam.inverseStatus2 &= ~(1<<0); /* capacity invers */
    //}
//}
//
//void HoTTv4::gamSetInputVoltage(uint16_t voltage) {
    //gam.inputVoltage = voltage;
    //if (cfg->alarmVoltage && gam.inputVoltage < cfg->minVoltage) {
	//gam.inverseStatus2 |=   1<<1; /* power invers */
    //} else {
	//gam.inverseStatus2 &= ~(1<<1); /* capacity invers */
    //}
//}
//
//void HoTTv4::gamSetCapacity(uint16_t capacity) {
    //gam.capacity = capacity;
    //if (cfg->alarmCapacity && gam.capacity < cfg->minCapacity) {
	//gam.inverseStatus2 |=   1<<5; /* capacity invers */
    //} else {
	//gam.inverseStatus2 &= ~(1<<5); /* capacity invers */
    //}
//}
//
//void HoTTv4::gamUpdateAlarmTone() {
    //if (gam.inverseStatus2)
	//gam.alarmTone = HoTTv4NotificationUndervoltage;
    //else
	//gam.alarmTone = 0;
//}
//
//void HoTTv4::gamSetSpeed(uint16_t speed) {
    //gam.speed = speed;
//}
//
//void HoTTv4::gamSetPressure(uint8_t pres) {
    //gam.pressure = pres;
//}
//
//void HoTTv4::gamText(uint8_t c) {
    //uicText(c, HOTTV4_GENERAL_AIR_SENSOR_TEXT_ID);
//}
//
//bool HoTTv4::gamSend() {
    //sendData(&gam.startByte, sizeof(gam));
    //return true;
//}
//
//
//----------------------- Text -----------------
void HoTTv4::txtClear() {
    memset(txt.text, ' ', sizeof(txt.text));
}

//void HoTTv4::txtPrint(uint8_t row, uint8_t col, uint8_t len,
		      //const char* str, bool pgm, bool invert) {
    //uint8_t* dst = txt.text[row]+col;
    //if (col+len>21) len = 21 - col; /* just for sure */
    //while(len--) {
	//uint8_t c = pgm ? pgm_read_byte(str) : *str;
	//if (c) {
	    //str++;
	    //if (invert) { c = c | 0x80; }  // some Up/down key do not work if there is inverted text ?
	//} else {
	    //c = ' ';
	//}
	//*dst++ = c;
    //}
//}
//
//bool HoTTv4::txtSend() {
    //sendData(&txt.startByte, sizeof(txt));
    //return true;
//}
//
////----------------------- Telemetry send -----------------
//bool HoTTv4::telemetrySend() {
    //bool send = false;
    //if (ss.available() < 2) return send;
//
    //uint8_t c = ss.read();
    //switch (c) {
	//case 0x80: /* binary */
	    //c = ss.read();
	    //switch (c) {
		//case HOTTV4_GENERAL_AIR_SENSOR_ID:    send = gamSend();   break;
	    //}
	    //break;
	//case 0x7F: /* text */
	    //c = ss.read();
	    //switch (c & 0xF0) {
		//case HOTTV4_GENERAL_AIR_SENSOR_TEXT_ID:    gamText(c); break;
	    //}
	    //send = txtSend();
	    //break;
	//default:
	    //break;
    //}
    //return send;
//}
//
////----------------------- Telemetry send -----------------
//void HoTTv4::dbgText() {
    //extern int __heap_start, *__brkval;
    //int v;
    //uint16_t avail = (int) &v - (__brkval == 0 ? (int) &__heap_start
					       //: (int) __brkval);
    //txtClear();
//
    //char buf[6];
    //itoa16(buf, avail,0);				/* 96 */
    //txtPrint(0, 0, 7, buf, false, false);
    //itoa16(buf, sizeof(Led),0);				/*  9 */
    //txtPrint(1, 0, 7, buf, false, false);
    //itoa16(buf, sizeof(Config),0);			/* 15 */
    //txtPrint(2, 0, 7, buf, false, false);
    //itoa16(buf, sizeof(HoTTv4),0);			/* 262 */
    //txtPrint(3, 0, 7, buf, false, false);
    //itoa16(buf, sizeof(HoTTv4GeneralAirModule_t),0);	/* 44 */
    //txtPrint(4, 0, 7, buf, false, false);
    //itoa16(buf, sizeof(HoTTv4TextModule_t),0);		/* 172 */
    //txtPrint(5, 0, 7, buf, false, false);
    //itoa16(buf, cfg->calFactor,0);			/* near CalResol ? */
    //txtPrint(6, 0, 7, buf, false, false);
//}
