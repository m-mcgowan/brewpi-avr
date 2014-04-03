/*
 * DataStream.cpp
 *
 * Created: 30/03/2014 15:33:43
 *  Author: mat
 */ 

#include "Brewpi.h"
#include "DataStream.h"

	
bool BufferDataOut::write(uint8_t data) {
	if (pos<size) {
		buffer[pos++] = data;
		return true;
	}
	return false;
}

// todo - given that the arduino is the most space-constrained, then better to make the default
// little endian.
void writeBytes(void* data, uint8_t size, DataOut& out)
{
	#if ARDUINO
	// arduino is little-endian
	uint8_t* buf = (uint8_t*)data;
	for (int i=size; i-->0; )
		out.write(buf[i]);
	#else
	// intel is big endian
	out.writeBuffer((uint8_t*)data, size);
	#endif
}


void readMaskedBytes(void* _data, uint8_t size, DataIn& in, DataIn& mask)
{
	uint8_t* data = (uint8_t*)_data;
	#if ARDUINO
	for (uint8_t i=size; i-->0; i++)
	#else
	for (uint8_t i=0; i<size; i++)
	#endif
	{
		uint8_t d = in.next();
		uint8_t m = mask.next();
		data[i] = (d&m) | (data[i] & ~m);	
	}
}