#pragma once

#include "DataStream.h"
#include "Values.h"
#include "EepromAccess.h"
#include "DataStreamEeprom.h"

/**
 * A read-write value in eeprom
 */
class EepromBaseValue : public Value, public StreamWritable {

protected:
		void _readTo(DataOut& out, eptr_t offset, uint8_t size)
		{
			EepromDataIn in(offset, size);
			in.push(out, size);
		}
		
		void _writeFrom(DataIn& in, eptr_t offset, uint8_t size)
		{
			EepromDataOut out(offset, size);
			in.push(out, size);
		}		
		
		object_t objectType() { return otValue | otWritableFlag; }
			
};

template <uint8_t _size> class EepromStreamValue : public EepromBaseValue
{
	protected:
		eptr_t _offset;
					
	public:
		EepromStreamValue(eptr_t offset) : _offset(offset) {}

		void readTo(DataOut& out) {
			_readTo(out, eeprom_offset(), streamSize());
		}
	
		void writeFrom(DataIn& out) {
			_writeFrom(out, eeprom_offset(), streamSize());
		}

		eptr_t eeprom_offset() { return _offset; }
		uint8_t streamSize() { return _size; }


};

template <class T, int _size=sizeof(T)> class EepromValue : public EepromStreamValue<_size>
{
	public:
		
		EepromValue(eptr_t offset) : EepromStreamValue<_size>(offset) {}
		
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

class EepromDynamicValue : public EepromBaseValue
{
	private:
		eptr_t _offset;
		uint8_t _size;
		
	public:	
		EepromDynamicValue(eptr_t offset, uint8_t size) : _offset(offset), _size(size) {}
	
		void writeFrom(DataIn& in) {
			_writeFrom(in, _offset, _size);
		}
		
		void readTo(DataOut& out) {
			_readTo(out, _offset, _size);
		}
		
		uint8_t streamSize() { return _size; }
};

	