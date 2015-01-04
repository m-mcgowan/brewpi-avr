//
//    FILE: dht.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.10
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//     URL: http://arduino.cc/playground/Main/DHTLib
//
// HISTORY:
// see dht.cpp file
//

#ifndef dht_h
#define dht_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include "limits.h"

#define DHT_LIB_VERSION "0.1.10"

#define DHTLIB_OK                0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2


#define DHTLIB_DHT11_WAKEUP     18
#define DHTLIB_DHT22_WAKEUP     1

// fixed point 12.4 signed
typedef int16_t dht_fixed;
#define DHT_FIXED_SHIFT 4
#define DHTLIB_INVALID_VALUE    INT_MIN		// mdm - was -999

class dht
{
	public:
	void prepare(uint8_t pin);
	
	int read11(uint8_t pin);
	int read21(uint8_t pin);
	int read22(uint8_t pin);
	
	dht_fixed humidity;
	dht_fixed temperature;

	private:
	uint8_t bits[5];  // buffer to receive data
	int read(uint8_t pin, uint8_t wakeupDelay);
	
	bool waitForPin(uint8_t state) {
		
	}
};
#endif
//
// END OF FILE
//