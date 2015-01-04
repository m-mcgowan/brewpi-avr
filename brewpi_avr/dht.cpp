//
//    FILE: dht.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.10
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//     URL: http://arduino.cc/playground/Main/DHTLib
//
// HISTORY:
// MDM: removed use of double and instead use fixed point values. Added separate prepare() method that has the wakeup delay.
// 0.1.10 optimized faster WAKEUP + TIMEOUT
// 0.1.09 optimize size: timeout check + use of mask
// 0.1.08 added formula for timeout based upon clockspeed
// 0.1.07 added support for DHT21
// 0.1.06 minimize footprint (2012-12-27)
// 0.1.05 fixed negative temperature bug (thanks to Roseman)
// 0.1.04 improved readability of code using DHTLIB_OK in code
// 0.1.03 added error values for temp and humidity when read failed
// 0.1.02 added error codes
// 0.1.01 added support for Arduino 1.0, fixed typos (31/12/2011)
// 0.1.0 by Rob Tillaart (01/04/2011)
//
// inspired by DHT11 library
//
// Released to the public domain
//

#include "dht.h"

// max timeout is 100usec.
// For a 16Mhz proc that is max 1600 clock cycles
// loops using TIMEOUT use at least 4 clock cycli
// so 100 us takes max 400 loops
// so by dividing F_CPU by 40000 we "fail" as fast as possible
#define TIMEOUT (F_CPU/40000)


/////////////////////////////////////////////////////
//
// PUBLIC
//

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
int dht::read11(uint8_t pin)
{
	humidity    = DHTLIB_INVALID_VALUE; // invalid value, or is NaN prefered?
	temperature = DHTLIB_INVALID_VALUE; // invalid value

	// READ VALUES
	int rv = read(pin, DHTLIB_DHT11_WAKEUP);
	if (rv != DHTLIB_OK)
	{
		return rv;
	}

	// TEST CHECKSUM
	// bits[1] && bits[3] both 0
	uint8_t sum = bits[0] + bits[2];
	if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;

	// CONVERT AND STORE
	humidity    = dht_fixed(bits[0])<<DHT_FIXED_SHIFT;  // bits[1] == 0;
	temperature = dht_fixed(bits[2])<<DHT_FIXED_SHIFT;  // bits[3] == 0;

	return DHTLIB_OK;
}

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
int dht::read21(uint8_t pin)
{
	// dataformat & wakeup identical to DHT22
	return read22(pin);
}

dht_fixed convert(uint8_t v1, uint8_t v2) {
	// the maximum value is the 100% encoded as 1000. Shift 5 places is 32000 (close to max for a signed value).
	// then divide by 20 - (2*10) because we shifted one extra place (for precision in division) and the
	// original value is multiplied by 10. 
	return (dht_fixed(word(v1, v2)) << (DHT_FIXED_SHIFT+1))/20;
}

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
int dht::read22(uint8_t pin)
{
	humidity    = DHTLIB_INVALID_VALUE;  // invalid value, or is NaN prefered?
	temperature = DHTLIB_INVALID_VALUE;  // invalid value
	
	// READ VALUES
	int rv = read(pin, DHTLIB_DHT22_WAKEUP);
	if (rv != DHTLIB_OK)
	{
		return rv; // propagate error value
	}

	// TEST CHECKSUM
	uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];
	if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;

	// CONVERT AND STORE
	// value reported as 10x actual value. Shift more to obtain more precision with the divide
	// max value is 1000, shifted 5 places is 32000.
	humidity = convert(bits[0], bits[1]);
	temperature = convert(bits[2] & 0x80 ? -(bits[2] & 0x7F) : bits[2], bits[3]);
	return DHTLIB_OK;
}

void dht::prepare(uint8_t pin) {
	// REQUEST SAMPLE
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

/////////////////////////////////////////////////////
//
// PRIVATE
//

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_TIMEOUT
int dht::read(uint8_t pin, uint8_t wakeupDelay)
{
	// prepare() should have been called before this
	
	// INIT BUFFERVAR TO RECEIVE DATA
	uint8_t mask = 128;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (uint8_t i = 0; i < 5; i++) bits[i] = 0;

	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	// GET ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = TIMEOUT;
	while(digitalRead(pin) == LOW)
	{
		if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;
	}

	loopCnt = TIMEOUT;
	while(digitalRead(pin) == HIGH)
	{
		if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;
	}

	// READ THE OUTPUT - 40 BITS => 5 BYTES
	for (uint8_t i = 0; i < 40; i++)
	{
		loopCnt = TIMEOUT;
		while(digitalRead(pin) == LOW)
		{
			if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;
		}

		unsigned long t = micros();

		loopCnt = TIMEOUT;
		while(digitalRead(pin) == HIGH)
		{
			if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;
		}

		if ((micros() - t) > 40) bits[idx] |= mask;
		mask >>= 1;
		if (mask == 0)   // next byte?
		{
			mask = 128;
			idx++;
		}
	}
	return DHTLIB_OK;
}
//
// END OF FILE
//