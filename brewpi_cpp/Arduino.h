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


#ifndef ARDUINO_H
#define	ARDUINO_H

#include <algorithm>
#include <iostream>
#include <string.h>

#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"
#include "limits.h"
#include "math.h"
#include "print.h"


inline uint8_t pgm_read_byte(const void* pv) {
    return *(uint8_t*)pv;
}


#define strcpy_P strcpy
#define strlcpy_P strncpy
#define strlen_P strlen
#define sprintf_P sprintf
#define strcmp_P strcmp
#define memcpy_P memcpy
#define vsnprintf_P vsnprintf
#define ltoa itoa                       // 32-bit platform itoa is good enough
#define _delay_us(us)       // no cross platform us delay

inline char *strchrnul(char *s, int c)
{
       char *ptr = strchr(s, c);
       if (!ptr) {
               ptr = strchr(s, '\0');
       }
       return ptr;
}

/*
namespace sc = std::chrono;

inline uint32_t millis() {             
    auto time = sc::system_clock::now(); // get the current time
    auto since_epoch = time.time_since_epoch(); // get the duration since epoch
    auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
    long now = millis.count(); // just like java (new Date()).getTime();
}
*/

#include "timems.h"

inline uint32_t millis() { return (uint32_t)millisSinceStartup(); } 

#define PROGMEM         // makes no sense in a unified memory architecture
#define PSTR(x) x


typedef uint8_t DeviceAddress[8];

inline boolean isHexadecimalDigit(int c)
{
  return ( isxdigit (c) == 0 ? false : true);
}


typedef unsigned char byte;


using std::min;
using std::max;

#define TWO_PI 6.283185307179586476925286766559

inline double random(double max) {
    return (double(rand())*max/RAND_MAX);
}
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#endif	/* ARDUINO_H */

