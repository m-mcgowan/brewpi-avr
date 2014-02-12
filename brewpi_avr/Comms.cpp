/*
 * Comms.cpp
 *
 * Created: 05/02/2014 22:17:25
 *  Author: mat
 */ 

#include "Brewpi.h"
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

class CommsIn : public DataIn
{	
	bool hasNext() { return comms; }			// hasNext true if stream is still open.
	uint8_t next() { return comms.read(); }	
	uint8_t peek() { return comms.peek(); }
};

class CommsOut : public DataOut
{
	bool write(uint8_t data) { comms.write(data); return true; }	
};

// low-level binary in/out streams
CommsIn commsIn;		
CommsOut commsOut;

/*
 * Filters out non-significant text - comment markers, whitespace, unrecognized characters.
 * The stream automatically closes on newline. 
 */
class TextIn : public DataIn {
	DataIn*	_in;
	uint8_t data;
	bool hasData;
	int8_t commentLevel;	// -1 indicates end of stream

	void fetchNextData();
	
public:
	TextIn(DataIn& in) 
	: _in(&in), data(0), hasData(0), commentLevel(0) {}
		
	bool hasNext() 
	{ 
		fetchNextData();
		return hasData;
	}
	
	uint8_t next()  
	{ 
		fetchNextData();
		hasData = false;
		return data;
	}
	
	uint8_t peek() 
	{
		fetchNextData();
		return data;
	}	
};

/**
 * Fetches the next significant data byte from the stream.
 * Sets hasData and data. 
 */
void TextIn::fetchNextData() {
	
	while (commentLevel>=0 && !hasData && _in->hasNext()) {
		data = 0xFF;
		uint8_t d = _in->next();
		if (d=='[') commentLevel++;			
		else if (d==']') commentLevel--;
		else if (d=='\n') { commentLevel = -1; data = 0; }
		else if (!commentLevel && isHexadecimalDigit(d)) {
			hasData = true;
			data = d;
		}
	}	
}

/**
 * Converts a hex digit to the corresponding binary value.
 */
uint8_t h2d(unsigned char hex)
{	
	if (hex > '9')
		hex -= 7; // 'A' is 0x41, 'a' is 0x61. -7 =  0x3A, 0x5A	
	return (hex & 0xf);
}

unsigned char d2h(uint8_t bin)
{
	return bin+(bin>9 ? 'A'-10 : '0');	
}

/*
 * Converts pairs of hex digit characters into the corresponding binary value.
 */
class HexTextToBinaryIn : public DataIn
{
	DataIn* _text;	// store as pointer to avoid non-POD warnings
	uint8_t char1;	// Text character for upper nibble
	uint8_t char2;	// Text character for lower nibble

	void fetchNextByte();
	
public:
	HexTextToBinaryIn(DataIn& text) : _text(&text), char1(0), char2(0) {}
	
	bool hasNext() {
		fetchNextByte();
		return char2;
	}
	
	uint8_t peek() {
		fetchNextByte();
		return (h2d(char1)<<4) | h2d(char2);		
	}
	
	uint8_t next()  {
		uint8_t r = peek();
		char1 = 0; char2 = 0;
		return r;
	}
};

/**
 * Fetches the next byte from the stream.
 */
void HexTextToBinaryIn::fetchNextByte() 
{
	DataIn& _text = *this->_text;
	if (!_text.hasNext())
		return;
		
	if (!char1) {
		char1 = _text.next();
	}

	if (!_text.hasNext())
		return;

	if (!char2) {
		char2 = _text.next();
	}
}

class BinaryToHexTextOut : public DataOut {

	DataOut* _out;		

public:

	BinaryToHexTextOut(DataOut& out) : _out(&out) {}

	/**
	 * Annotations are written as is to the stream, surrouned by annotation marks.
	 */
	void writeAnnotation(const char* data) {
		_out->write('[');
		_out->writeBuffer(data, strlen(data));
		_out->write(']');
	}

	/**
	 * Data is written as hex-encoded
	 */
	bool write(uint8_t data) {
		_out->write(d2h((data&0xF0)>>4));
		_out->write(d2h((data&0xF)));
		_out->write(' ');
		return true;
	}
	
	/**
	 * Rather than closing the global stream, write a newline to signify the end of this command.
	 */
	void close() {
		_out->write('\n');
	}
};

BinaryToHexTextOut hexOut(commsOut);
DataOut& Comms::hexOut = hexOut;

void Comms::receive() {
		
	while (comms.available()>0) {			// there is some data ready to be processed											// form this point on, the system will block waiting for a complete command or newline.
		TextIn textIn(commsIn);
		HexTextToBinaryIn hexIn(textIn);
		BinaryToHexTextOut hexOut(commsOut);
		if (hexIn.hasNext())				// ignore blank newlines, annotations etc..
			handleCommand(hexIn, hexOut);
		hexOut.close();
	}	
}

