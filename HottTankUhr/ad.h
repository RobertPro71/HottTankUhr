/* 
* ad.h
*
* Created: 21.05.2020 13:05:29
* Author: Robert
*/


#ifndef __AD_H__
#define __AD_H__

#include <avr/io.h>

#define VOLTAGE   0
#define CURRENT   1

struct channel_st{
	uint8_t ad_id;
	uint16_t raw;
	uint16_t scaleInt;
	float real;
	uint16_t offset;
	float scale;
	bool newval;
};

class ad
{
//functions
public:
	void init();
	
	void OnAdInterrupt();
	void OnAdTimerInterrupt();
private:
	void StartTimer();

//variables
public:
  channel_st ChannelList[2];
private:
	uint8_t measureChannelId;

}; //ad

#endif //__AD_H__
