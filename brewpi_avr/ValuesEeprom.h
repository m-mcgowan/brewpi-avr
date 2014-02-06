#pragma once

#include "DataStream.h"
#include "Values.h"
#include "EepromAccess.h"

struct EepromBaseData
{
	protected:
		eptr_t _offset;
		uint8_t _length;

	public:		
		EepromBaseData(eptr_t offset, uint8_t length)
		: _offset(offset), _length(length) {}
	
		eptr_t offset() { return _offset; }
		eptr_t length() { return _length; }
};

struct EepromDataOut : public DataOut, EepromBaseData
{
	EepromDataOut(eptr_t offset, uint8_t length)
	: EepromBaseData(offset, length) {}

	void write(uint8_t value) {
		if (_length) {
			eepromAccess.writeByte(_offset++, value);
			_length--;
		}
	}
	void close() {
		_length = 0;
	}
};

struct EepromDataIn : public DataIn, EepromBaseData
{
	EepromDataIn(eptr_t offset, uint8_t length)
		: EepromBaseData(offset, length) {}
		
	
	bool hasNext() { return _length; }
	uint8_t peek() { return eepromAccess.readByte(_offset); }
		
	uint8_t next() {
		uint8_t result = 0;
		if (_length) {
			_length--;
			result = eepromAccess.readByte(_offset++);
		}
		return result;
	}
};
	
class EepromBaseValue : public AbstractStreamValue {

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
};

template <uint8_t _size> class EepromStreamValue : public EepromBaseValue
{
	protected:
		eptr_t _offset;
					
	public:
		EepromStreamValue(eptr_t offset) : _offset(offset) {}

		void readTo(DataOut& out) {
			_readTo(out, eeprom_offset(), size());
		}
	
		void writeFrom(DataIn& out) {
			_writeFrom(out, eeprom_offset(), size());
		}

		uint8_t size() { return _size; }
		eptr_t eeprom_offset() { return _offset; }
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
};

	