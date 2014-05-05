/*
 * Copyright 2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Brewpi.h"
#include "Ticks.h"


// return time that has passed since timeStamp, take overflow into account
ticks_seconds_t ExternalTicks::timeSince(ticks_seconds_t previousTime){
	ticks_seconds_t currentTime = seconds();
	return ::timeSince(currentTime, previousTime);
}

#if BREWPI_VIRTUAL
// return time that has passed since timeStamp, take overflow into account
ticks_seconds_t DesktopTicks::timeSince(ticks_seconds_t previousTime){
	ticks_seconds_t currentTime = seconds();
	return ::timeSince(currentTime, previousTime);
}


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void mySleep(uint32_t sleepUs)
{
#ifdef _WIN32
    Sleep(sleepUs/1000);
#else
    usleep(sleepUs);   // usleep takes sleep time in us
#endif  
}

void DesktopDelay::microseconds(uint32_t micros) 
{        
    mySleep(micros);
}
#endif




#ifdef ARDUINO

// return time that has passed since timeStamp, take overflow into account
ticks_seconds_t HardwareTicks::timeSince(ticks_seconds_t previousTime){
	ticks_seconds_t currentTime = seconds();
	return ::timeSince(currentTime, previousTime);
}

ticks_seconds_t HardwareTicks::seconds() { return ::millis()/1000; }
	

void HardwareDelay::millis(uint16_t millis) { ::delay(millis); }
	
void HardwareDelay::seconds(uint16_t seconds)	{ millis(seconds<<10); }

#endif