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
