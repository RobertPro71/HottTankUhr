/* 
* ahcounter_cl.h
*
* Created: 23.05.2020 22:50:32
* Author: Robert
*/


#ifndef __AHCOUNTER_CL_H__
#define __AHCOUNTER_CL_H__


class ahcounter_cl
{
//functions
public:
  void init();
	void count(float current);
	float val();
private:

//variables
public:
private:
  float AhCounter;
	float scale;

}; //ahcounter_cl

#endif //__AHCOUNTER_CL_H__
