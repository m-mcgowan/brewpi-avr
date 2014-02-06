/*
 * Comms.cpp
 *
 * Created: 05/02/2014 22:17:25
 *  Author: mat
 */ 

#include "brewpi.h"
#include "Comms.h"
#include "Commands.h"

// Rename Serial to piStream, to abstract it for later platform independence

#if BREWPI_EMULATE
class MockSerial : public Stream
{
	public:
	void print(char c) {}
	void print(const char* c) {}
	void printNewLine() {}
	void println() {}
	int read() { return -1; }
	int available() { return -1; }
	void begin(unsigned long) {}
	size_t write(uint8_t w) { return 1; }
	int peek() { return -1; }
	void flush() { };
	operator bool() { return true; }
};

static MockSerial comms;

#elif !defined(ARDUINO)
StdIO comms;
#else
#define comms Serial
#endif

void Comms::init() {
	comms.begin(57600);
}

uint8_t readByte()
{
	return 0;	
}

class CommsIn : public DataIn
{
	bool hasNext() { return comms.available(); }	
	uint8_t next() { return comms.read(); }	
	uint8_t peek() { return comms.peek(); }
};

class CommsOut : public DataOut
{
	void write(uint8_t data) { comms.write(data); }
	void write(uint8_t* data, uint8_t len) { comms.write(data, len); }
	void close() { }
};

CommsIn commsIn;
CommsOut commsOut;

void Comms::receive() {
		
	while (comms.available()>0) {
		handleCommand(commsIn, commsOut);
	}
	
}

