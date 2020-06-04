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
#include <avr/interrupt.h>

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
    bin.value.startByte      = HOTTV4_START_BIN;
    bin.value.sensorID       = HOTTV4_GENERAL_AIR_SENSOR_ID;
    bin.value.sensorTextID   = HOTTV4_GENERAL_AIR_SENSOR_TEXT_ID;
    bin.value.endByte        = HOTTV4_STOP;
    bin.value.lowestCellVoltage = 0;
    bin.value.lowestCellNumber  = 0;

    txt.value.startByte      = HOTTV4_START_TXT;
    txt.value.sensorTextID   = HOTTV4_ELECTRICAL_AIR_SENSOR_TEXT_ID;
    txt.value.endByte        = HOTTV4_STOP;

    txtClear();
    UartInit();
		Protstatus = WaitFirstByte;
}

void HoTTv4::UartInit(){
	PORTB.DIRSET = PIN_TX_bm;
	PORTB.OUTSET = PIN_TX_bm;
	
	uint32_t UBR_VAL =  (((8 * F_CPU) / HOTTBAUD) + 1) / 2;

	USART0.BAUD = (uint16_t)(UBR_VAL);	
	
	USART0.CTRLC =
		USART_CMODE_ASYNCHRONOUS_gc | // Mode: Asynchronous[default]
		USART_PMODE_DISABLED_gc | // Parity: None[default]
		USART_SBMODE_1BIT_gc | // StopBit: 1bit[default]
		USART_CHSIZE_8BIT_gc; // CharacterSize: 8bit[default]

	USART0.CTRLA =
		USART_RXCIE_bm | // Enable RX interrupt
		USART_TXCIE_bm;  // Enable TX interrupt

	USART0.CTRLB =
		USART_RXEN_bm | // Start Receiver
		USART_TXEN_bm | // Start Transmiter
		USART_RXMODE_NORMAL_gc; // Receiver mode is Normal USART & 1x-speed
		
	sei();
}

void HoTTv4::TimerInit()
{}

void HoTTv4::TimerStart(uint16_t mstime)
{
  TCB0.CCMP    = mstime;				//Time this counter run
  TCB0.CNT     = 0;							//start from zero
  TCB0.CTRLA   = TCB_ENABLE_bm;	//start timer
  TCB0.INTCTRL = TCB_CAPT_bm;   //enable interrupt
}

void HoTTv4::StartIdleLine(HottTransmittMode_e Mode)
{
	if(Mode == TModeBin) LastByteIndex = sizeof(HottBin_u);
	if(Mode == TModeTxt) LastByteIndex = sizeof(HottTxt_u);
	SendByteIndex = 0;
	UartRcvCounter = 0;
	Protstatus = WaitIdleLine;
	TimerStart(ms_5_c);
	DebugBlink(1);
}

void HoTTv4::OnRcvInterrupt(){
	//Receive a byte
	uint8_t recByte = USART0.RXDATAL;
	
	switch (Protstatus)
	{
		case WaitFirstByte:
			if(recByte == HOTTV4_REQUEST_BIN) Protstatus = WaitSecondByteBin;
			if(recByte == HOTTV4_REQUEST_TXT) Protstatus = WaitSecondByteTxt;
			break;
		case WaitSecondByteBin:
			if(recByte == HOTTV4_GENERAL_AIR_SENSOR_ID){
				TransmittMode = TModeBin;
				StartIdleLine( TModeBin );
			}
			else{
				Protstatus = WaitFirstByte;
			}
			break;
		case WaitSecondByteTxt:
			if((recByte & HOTTV4_MASK_TEXT_ID) == HOTTV4_GENERAL_AIR_SENSOR_TEXT_ID){
				TransmittMode = TModeTxt;
				ButtonRequest = (recByte & HOTTV4_TEXT_BUTTON_MASK);
				StartIdleLine( TModeTxt );
			}
			else{
				Protstatus = WaitFirstByte;
			}
			break;
		case WaitIdleLine:
			UartRcvCounter++;
			break;
		//default:
			//Protstatus = ???
		default: break;
	}
}

void HoTTv4::OnSndInterrupt()
{
	USART0.STATUS |= USART_TXCIF_bm;
	
	if(Protstatus != LastByte){
		TimerStart	(ms_1_c);
	}
	else{
		Protstatus = WaitFirstByte;
	}
}

void HoTTv4::OnUartTimerInterrupt()
{
  //Timer stoppen
  TCB0.CTRLA = 0;
  TCB0.INTFLAGS = TCB_CAPT_bm;

	DebugBlink(2);
	
	switch(Protstatus)
	{
		case WaitIdleLine:
			if(UartRcvCounter != 0){
	DebugBlink(3);
				Protstatus = WaitFirstByte;
				break;
			}
			else{
	DebugBlink(4);
				crc = 0;
				if (TransmittMode == TModeBin){
					Protstatus = SendBin;
				}
				else{
					Protstatus = SendTxt;
				}
			}
		case SendBin:
	DebugBlink(5);
			if(SendByteIndex < LastByteIndex){
				sendByte(bin.byte[SendByteIndex++]); 
	DebugBlink(6);
			}
			else{
				USART0.TXDATAL = uint8_t(crc & 0x00FF);
				Protstatus = LastByte;
			}
			break;
		case SendTxt:
			if(SendByteIndex < LastByteIndex){
				sendByte(bin.byte[SendByteIndex++]);
			}
			else{
				USART0.TXDATAL = uint8_t(crc & 0x00FF);
				Protstatus = LastByte;
			}
			break;
		default: break;
	}
}

void HoTTv4::sendByte(uint8_t data){
	crc += data;
	USART0.TXDATAL = data;
}

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
//void HoTTv4::gamSetBattery1(uint16_t volt) {
    //gam.battery1 = volt;
//}
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

//----------------------- Text -----------------
void HoTTv4::txtClear() {
    memset(txt.value.text, ' ', sizeof(txt.value.text));
}

void HoTTv4::txtDefault()
{
	txt.value.text[2][3] = 'H';
	txt.value.text[2][4] = 'a';
	txt.value.text[2][5] = 'l';
	txt.value.text[2][6] = 'l';
	txt.value.text[2][7] = 'o';
	txt.value.text[2][8] = ' ';
	txt.value.text[2][9] = '0';
	txt.value.text[2][10] = '.';
	txt.value.text[2][10] = '1';
}

void HoTTv4::DebugBlink(uint8_t count)
{
	for (;count;count--)
	{
		PORTB.OUTSET = PIN_DEBUG_bm;
		PORTB.OUTCLR = PIN_DEBUG_bm;
	}
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
