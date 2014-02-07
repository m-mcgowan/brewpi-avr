/*
 * DataStream.h
 *
 * Created: 06/02/2014 10:05:22
 *  Author: mat
 */ 

#pragma once

#ifndef DATASTREAM_ANNOTATIONS
#define DATASTREAM_ANNOTATIONS DEBUG
#endif


struct DataOut
{
	#ifdef STREAM_ANNOTATIONS
	virtual void writeAnnotation(PCSTR data) {}
	#endif
	
	virtual void write(uint8_t data)=0;
	virtual void write(uint8_t* data, uint8_t len) {
		while (len-->0) {
			write(*data++);
		}
	}
	virtual void close()=0;
};

struct BlackholeDataOut : public DataOut {	
	virtual void write(uint8_t data) { }
	virtual void close() {}
};

struct DataIn
{

	virtual bool hasNext() =0;
	virtual uint8_t next() =0;
	virtual uint8_t peek() =0;
		
	virtual ~DataIn() {}
		
	/*
	 * Unconditional read of {@code length} bytes. 
	 */
	void read(uint8_t* target, uint8_t length) {
		while (length-->0) {
			*target++ = next();
		}
	}
	
	void push(DataOut& out, uint8_t length) {
		while (length-->0 && hasNext()) {
			out.write(next());
		}
	}
};





#define WRITE_ANNOTATION_STR(out, value) \
	WRITE_ANNOTATION(out, PSTR(value))

#if DATASTREAM_ANNOTATIONS
	#define WRITE_ANNOTATION(out, value) \
		out->writeAnnotation(value);
#else
	#define WRITE_ANNOTATION(out, value)
#endif	