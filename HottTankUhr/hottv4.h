//----------------------------------------------------------------------
// HottV4
//
// Implementation of HoTT V4 protocol
//
// Author: Heinz Bruederlin
//----------------------------------------------------------------------
#ifndef hottv4_h
#define hottv4_h

#include <avr/io.h>

#define ALT_OFFSET        500
#define EAM_TEMP_OFFSET    20
#define GAM_TEMP_OFFSET    40

#define HOTTV4_START_BIN 0x7C
#define HOTTV4_START_TXT 0x7B
#define HOTTV4_STOP      0x7D

/** ###### HoTT buttons specifications ###### */
#define HOTTV4_TEXT_MODE_LEFT   0x07
#define HOTTV4_TEXT_MODE_DOWN   0x0D
#define HOTTV4_TEXT_MODE_UP     0x0B
#define HOTTV4_TEXT_MODE_RIGHT  0x0E
#define HOTTV4_TEXT_MODE_SET    0x09
#define HOTTV4_TEXT_MODE_NIL    0x0F

/** ###### HoTT module specifications ###### */

#define HOTTV4_VARIO_SENSOR_ID               0x89
#define HOTTV4_GPS_SENSOR_ID                 0x8A
#define HOTTV4_GENERAL_AIR_SENSOR_ID         0x8D
#define HOTTV4_ELECTRICAL_AIR_SENSOR_ID      0x8E

#define HOTTV4_VARIO_SENSOR_TEXT_ID          0x90
#define HOTTV4_GPS_SENSOR_TEXT_ID            0xA0
#define HOTTV4_GENERAL_AIR_SENSOR_TEXT_ID    0xD0
#define HOTTV4_ELECTRICAL_AIR_SENSOR_TEXT_ID 0xE0

typedef enum {
  HoTTv4NotificationErrorCalibration     = 0x01,
  HoTTv4NotificationErrorReceiver        = 0x02,
  HoTTv4NotificationErrorDataBus         = 0x03,
  HoTTv4NotificationErrorNavigation      = 0x04,
  HoTTv4NotificationErrorError           = 0x05,
  HoTTv4NotificationErrorCompass         = 0x06,
  HoTTv4NotificationErrorSensor          = 0x07,
  HoTTv4NotificationErrorGPS             = 0x08,
  HoTTv4NotificationErrorMotor           = 0x09,

  HoTTv4NotificationMaxTemperature       = 0x0A,
  HoTTv4NotificationAltitudeReached      = 0x0B,
  HoTTv4NotificationWaypointReached      = 0x0C,
  HoTTv4NotificationNextWaypoint         = 0x0D,
  HoTTv4NotificationLanding              = 0x0E,
  HoTTv4NotificationGPSFix               = 0x0F,
  HoTTv4NotificationUndervoltage         = 0x10,
  HoTTv4NotificationGPSHold              = 0x11,
  HoTTv4NotificationGPSHome              = 0x12,
  HoTTv4NotificationGPSOff               = 0x13,
  HoTTv4NotificationBeep                 = 0x14,
  HoTTv4NotificationMicrocopter          = 0x15,
  HoTTv4NotificationCapacity             = 0x16,
  HoTTv4NotificationCareFreeOff          = 0x17,
  HoTTv4NotificationCalibrating          = 0x18,
  HoTTv4NotificationMaxRange             = 0x19,
  HoTTv4NotificationMaxAltitude          = 0x1A,

  HoTTv4Notification20Meter              = 0x25,
  HoTTv4NotificationMicrocopterOff       = 0x26,
  HoTTv4NotificationAltitudeOn           = 0x27,
  HoTTv4NotificationAltitudeOff          = 0x28,
  HoTTv4Notification100Meter             = 0x29,
  HoTTv4NotificationCareFreeOn           = 0x2E,
  HoTTv4NotificationDown                 = 0x2F,
  HoTTv4NotificationUp                   = 0x30,
  HoTTv4NotificationHold                 = 0x31,
  HoTTv4NotificationGPSOn                = 0x32,
  HoTTv4NotificationFollowing            = 0x33,
  HoTTv4NotificationStarting             = 0x34,
  HoTTv4NotificationReceiver             = 0x35,
} HoTTv4Notification;

struct HoTTv4GeneralAirModule_t {
  uint8_t startByte;         /* Byte  1: 0x7C */
  uint8_t sensorID;          /* Byte  2: HOTT_GENERAL_PACKET_ID */
  uint8_t alarmTone;         /* Byte  3: # of beeps */
  uint8_t sensorTextID;      /* Byte  4: 0xD0 */
  uint8_t inverseStatus1;    /* Byte  5: */
  uint8_t inverseStatus2;    /* Byte  6: */
  uint8_t voltageCell1;      /* Byte  7: Voltage * 50 */
  uint8_t voltageCell2;      /* Byte  8: */
  uint8_t voltageCell3;      /* Byte  9: */
  uint8_t voltageCell4;      /* Byte 10  */
  uint8_t voltageCell5;      /* Byte 11: */
  uint8_t voltageCell6;      /* Byte 12: */
  uint16_t battery1;         /* Byte 13+14: LSB/MSB 51 = 5,1V */
  uint16_t battery2;         /* Byte 15+16: LSB/MSB 51 = 5,1V */
  uint8_t temperature1;      /* Byte 17: 64 = 24°C, 0 = -40°C */
  uint8_t temperature2;      /* Byte 18: 64 = 24°C, 0 = -40°C */
  uint8_t fuelPercent;       /* Byte 19: */
  int16_t fuelCapacity;      /* Byte 20+21: */
  uint16_t rpm;              /* Byte 22+23: */
  int16_t altitude;          /* Byte 24+25: */
  uint16_t m1s;              /* Byte 26+27: 3000 = 0 */
  uint8_t  m3s;              /* Byte 28: 120 = 0 */
  uint16_t current;          /* Byte 29+30: 1 = 0.1A */
  uint16_t inputVoltage;     /* Byte 31+32: 66  = 6,6V */
  uint16_t capacity;         /* Byte 33+33: 1  = 10mAh */
  uint16_t speed;            /* Byte 35: */
  uint8_t lowestCellVoltage; /* Byte 37: */
  uint8_t lowestCellNumber;  /* Byte 38: */
  uint16_t rpm2;             /* Byte 39+40: */
  uint8_t errorNumber;       /* Byte 41: */
  uint8_t pressure;          /* Byte 42: in 0.1bar 20=2.0bar */
  uint8_t version;           /* Byte 43: */
  uint8_t endByte;           /* Byte 44: 0x7D */
  //uint8_t chksum;          /* Byte 45: created on the fly */
};

struct HoTTv4ElectricAirModule_t {
  uint8_t startByte;      /* Byte 1: */
  uint8_t sensorID;       /* Byte 2: */
  uint8_t alarmTone;      /* Byte 3: Alarm */
  uint8_t sensorTextID;   /* Byte 4: */
  uint8_t alarmInverse1;  /* Byte 5: */
  uint8_t alarmInverse2;  /* Byte 6: */

  uint8_t cell1L;         /* Byte 7:  Low Voltage Cell 1 in 2mV steps */
  uint8_t cell2L;         /* Byte 8:  Low Voltage Cell 2 in 2mV steps */
  uint8_t cell3L;         /* Byte 9:  Low Voltage Cell 3 in 2mV steps */
  uint8_t cell4L;         /* Byte 10: Low Voltage Cell 4 in 2mV steps */
  uint8_t cell5L;         /* Byte 11: Low Voltage Cell 5 in 2mV steps */
  uint8_t cell6L;         /* Byte 12: Low Voltage Cell 6 in 2mV steps */
  uint8_t cell7L;         /* Byte 13: Low Voltage Cell 7 in 2mV steps */
  uint8_t cell1H;         /* Byte 14: High Voltage Cell 1 in 2mV steps */
  uint8_t cell2H;         /* Byte 15: High Voltage Cell 1 in 2mV steps */
  uint8_t cell3H;         /* Byte 16: High Voltage Cell 1 in 2mV steps */
  uint8_t cell4H;         /* Byte 17: High Voltage Cell 1 in 2mV steps */
  uint8_t cell5H;         /* Byte 18: High Voltage Cell 1 in 2mV steps */
  uint8_t cell6H;         /* Byte 19: High Voltage Cell 1 in 2mV steps */
  uint8_t cell7H;         /* Byte 20: High Voltage Cell 1 in 2mV steps */

  uint16_t battery1;      /* Byte 21+22: Bat 1 LSB/MSB 100mv steps */
  uint16_t battery2;      /* Byte 23+24: Bat 2 LSB/MSB 100mv steps */

  uint8_t temperature1;   /* Byte 25: Temp 1. 20 == 0C */
  uint8_t temperature2;   /* Byte 26: Temp 2. 20 == 0C */

  uint16_t altitude;      /* Byte 27+28: LSB/MSB Height. 500 == 0m */
  uint16_t current;       /* Byte 29+30: 1 = 0.1A */
  uint16_t driveVoltage;  /* Byte 31+32: 0.002V */
  uint16_t capacity;      /* Byte 33+34: mAh */
  uint16_t m2s;           /* Byte 35+35: LSB/MSB */
  uint8_t  m3s;           /* Byte 37: */

  uint16_t rpm;           /* Byte 38+39: LSB/MSB RPM. 300 == 3000rpm */
  uint8_t minutes;        /* Byte 40: */
  uint8_t seconds;        /* Byte 41: */
  uint8_t speed;          /* Byte 42: */

  uint8_t version;        /* Byte 43: */
  uint8_t endByte;        /* Byte 44: 0x7D */
  //uint8_t chksum;       /* Byte 45: created on the fly */
};

struct HoTTv4TextModule_t {
  uint8_t startByte;      /* Byte: 1 */
  uint8_t sensorTextID;   /* Byte: 2 */
  uint8_t alarm;          /* Byte: 3 */
  uint8_t text[8][21];    /* Byte: 4-171 */
  uint8_t endByte;        /* Byte: 172 */
  //uint8_t chksum;       /* Byte 173: created on the fly */
};

class Config;

class HoTTv4 {
public:
    HoTTv4();
		void setup();
		void UartInit();
		
    //void gamSetCellVoltage(uint8_t no, uint8_t volt);
    //void gamSetBattery1(uint16_t volt);
    //void gamSetBattery2(uint16_t volt);
    //void gamSetTemperature1(uint8_t temp);
    //void gamSetTemperature2(uint8_t temp);
    //void gamSetFuelPercent(uint8_t percent);
    //void gamSetFuelCapacity(int16_t capacity);
    //void gamSetRPM(uint16_t rpm);
    //void gamSetRPM2(uint16_t rpm);
    //void gamSetAltitude(uint16_t altitude);
    //void gamSetCurrent(uint16_t current);
    //void gamSetInputVoltage(uint16_t voltage);
    //void gamSetCapacity(uint16_t capacity);
    //void gamSetSpeed(uint16_t speed);
    //void gamSetPressure(uint8_t pres);
    //void gamText(uint8_t c);
    //void gamUpdateAlarmTone();
//
    //bool telemetrySend();

private:
    void UartEnableRx();
    void UartEnableTx();
    //void write(uint8_t c);
    //void sendData(uint8_t *data, uint8_t size);

    void txtClear();
    //void txtPrint(uint8_t row, uint8_t col, uint8_t len,
      //const char* str, bool pgm, bool invert);
    //bool txtSend();
//
    //bool gamSend();
    //void uicText(uint8_t c, uint8_t sensorID);
    //void dbgText();
//
    //uint8_t    row, col;
//
    //uint16_t   gam_m1;
    //uint32_t   gam_m1s;
    //uint16_t   gam_m3;
    //uint32_t   gam_m3s;
    HoTTv4GeneralAirModule_t  gam;
    HoTTv4TextModule_t        txt;
};

#endif
