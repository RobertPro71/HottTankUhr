/* 
* ahcounter_cl.cpp
*
* Created: 23.05.2020 22:50:32
* Author: Robert
*/


#include "ahcounter_cl.h"

void ahcounter_cl::init()
{
  AhCounter = 0;
	scale = (1000 / (float)(60L * 60 * 10));
}

void ahcounter_cl::count(float current)
{
  AhCounter += (current * scale); 
}

float ahcounter_cl::val()
{
  return AhCounter;
}
