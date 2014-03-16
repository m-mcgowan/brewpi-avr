#pragma once

#include "DataStream.h"
#include "Values.h"
#include "EepromAccess.h"
#include "DataStreamEeprom.h"

/**
 * Base class for a read-write value in eeprom. This class is responsible for moving the data
 * between eeprom and the stream. 
 */
class EepromBaseValue : public Value  {

protected:
		void _readTo(DataOut& out, eptr_t offset, uint8_t size)
		{
			EepromDataIn in;
			in.reset(offset, size);
			in.push(out, size);
		}
		
		void _writeFrom(DataIn& in, eptr_t offset, uint8_t size)
		{
			EepromDataOut out;
			out.reset(offset, size);
			in.push(out, size);
		}		
		
		object_t objectType() { return otValue | otWritableFlag; }
			
};

/**
 * Streams an eeprom value of a given fixed size.
 */
class EepromStreamValue : public EepromBaseValue
{
	protected:
		eptr_t _offset;
		uint8_t _size;
					
	public:
		EepromStreamValue(eptr_t offset, uint8_t size) : _offset(offset), _size(size) {}

		void readTo(DataOut& out) {
			_readTo(out, eeprom_offset(), streamSize());
		}
	
		void writeFrom(DataIn& out) {
			_writeFrom(out, eeprom_offset(), streamSize());
		}

		eptr_t eeprom_offset() { return _offset; }
		uint8_t streamSize() { return _size; }

};

/**
 * Provides state read/write (in addition to stream read/write) for an eeprom value. 
 */
template <class T, int _size=sizeof(T)> class EepromValue : public EepromStreamValue
{
	public:
		
		EepromValue(eptr_t offset) : EepromStreamValue(offset, _size) {}
		
		T read() {
			T result;
			EepromDataIn in(this->_offset, _size);
			in.read((uint8_t*)&result, _size);
			return result;
		}
		
		void write(T t) {
			EepromDataOut out(this->_offset, _size);
			out.write(&t, _size);
		}
		
};

/**
 * Provides a streamable value to eeprom. The size is dynamic, unlike EepromStreamValue, where the size is
 * known at compile time. 
 */
class EepromDynamicStreamValue : public EepromBaseValue
{
	private:
		eptr_t _offset;
		uint8_t _size;
		
	public:	
		EepromDynamicStreamValue(eptr_t offset, uint8_t size) : _offset(offset), _size(size) {}
	
		void writeFrom(DataIn& in) {
			_writeFrom(in, _offset, _size);
		}
		
		void readTo(DataOut& out) {
			_readTo(out, _offset, _size);
		}
		
		uint8_t streamSize() { return _size; }
};
	