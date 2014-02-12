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

typedef uint8_t stream_size_t;

struct DataOut
{
	#ifdef STREAM_ANNOTATIONS
	virtual void writeAnnotation(PCSTR data) {}
	#endif
	
	/**
	 * Writes a byte to the stream.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool write(uint8_t data)=0;

	/**
	 * Writes a number of bytes to the stream.
	 * @param data	The address of the data to write.
	 * @param len	The number of bytes to write.
	 * @return {@code true} if the byte was successfully written, false otherwise.
	 */
	virtual bool writeBuffer(const void* data, stream_size_t len) {
		const uint8_t* d = (const uint8_t*)data;
		while (len-->0) {
			if (!write(*d++))
				return false;
		}
		return true;
	}
	virtual void close() {}
};

struct BlackholeDataOut : public DataOut {	
	virtual bool write(uint8_t data) { return true; }	
};

/**
 * A data input stream.
 */
struct DataIn
{
	/*
	 * Determines if there is more data in this stream.
	 * The result from next/peek is undefined if this returns false.
	 * Note that this is not dependent upon time, but if the stream is still open.
	 */
	virtual bool hasNext() =0;
	virtual uint8_t next() =0;
	virtual uint8_t peek() =0;
	
	#if OBJECT_VIRTUAL_DESTRUCTOR	
	virtual ~DataIn() {}
	#endif
		
	/*
	 * Unconditional read of {@code length} bytes. 
	 */
	void read(void* t, uint8_t length) {
		uint8_t* target = (uint8_t*)t;
		while (length-->0 && hasNext()) {
			*target++ = next();
		}
	}
	
	void push(DataOut& out, uint8_t length) {
		while (length-->0 && hasNext()) {
			out.write(next());
		}
	}
};


/*
 * Reads data from a DataIn, and writes any fetched bytes to DataOut.
 */
class PipeDataIn : public DataIn
{
	DataIn* _in; 
	DataOut* _out;
	bool success;
	
public:	
	PipeDataIn(DataIn& in, DataOut& out) 
		: _in(&in), _out(&out), success(true)
	{		
	}
	
	bool pipeOk() { return success; }
	
	DataOut& pipeOut() { return *_out; }
	
	virtual uint8_t next() {
		uint8_t val = _in->next();
		bool result = _out->write(val);
		success = success && result;
		return val;
	}
	
	virtual bool hasNext() { return _in->hasNext(); }
	virtual uint8_t peek() { return _in->peek(); }
	
};



#define WRITE_ANNOTATION_STR(out, value) \
	WRITE_ANNOTATION(out, PSTR(value))

#if DATASTREAM_ANNOTATIONS
	#define WRITE_ANNOTATION(out, value) \
		out->writeAnnotation(value);
#else
	#define WRITE_ANNOTATION(out, value)
#endif	