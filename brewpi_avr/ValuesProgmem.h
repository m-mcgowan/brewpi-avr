#pragma once

/*
 * ValuesProgmem.h
 *
 * Created: 06/02/2014 10:35:19
 *  Author: mat
 */ 

#include "Values.h"

#ifdef ARDUINO
#include "avr/pgmspace.h"
#endif

class ProgmemStringValue : public Value
{	
	private:
		const char* value;
	
	public:
		ProgmemStringValue(const char* v) : value(v)  {}

		void readTo(DataOut& out) {
			const char* v = value;
			uint8_t b;
			do {
				b = pgm_read_byte(v++);
				out.write(b);
			} while (b);
		}
	
		uint8_t streamSize() {
			return strlen_P(value);
		}
};


