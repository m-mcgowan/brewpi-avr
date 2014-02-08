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


class ProgmemStringValue : public Readable<const char*>, public AbstractValue 
{	
	private:
		const char* value;
	
	public:
		ProgmemStringValue(const char* v) : value(v)  {}

		const char* read() {
			return value;
		}
	
		void readTo(DataOut& out) {
			const char* v = value;
			uint8_t b;
			do {
				b = pgm_read_byte(v++);
				out.write(b);
			} while (b);
		}
	
};


#endif // ARDUINO