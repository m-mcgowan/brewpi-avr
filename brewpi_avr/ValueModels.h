#pragma once
/*
 * ValueModels.h
 *
 * Created: 09/02/2014 16:01:39
 *  Author: mat
 */ 

#include "Brewpi.h"

/**
 * A streamable value whose data resizes externally.
 */
// TODO - factor ExternalValue, eeprom value and progmem value - all have address and size.
class ExternalValue : public Value, public StreamWritable
{
	void* _pValue;
	uint8_t _size;
	
public:

	ExternalValue(void* pValue, uint8_t size) {
		_pValue = pValue;
		_size = size;
	}
	
	void readTo(DataOut& out) {
		out.write((uint8_t*)_pValue, _size);
	}
	
	void writeFrom(DataIn& in) {
		in.read((uint8_t*)_pValue, _size);
	}
	
	uint8_t streamSize() {
		return _size;
	}
};

/**
 * Callback function for use by this container to establish values.
 * @param id	The index (greater or equal to 0) in the container to return the value of. When negative, returns	
 *   the number of items available. 
 * @return an object corresponding to the item at the given index, which may be {@code NULL}. 
 *   When {@code id} is negative, returns the number of items, cast as a pointer.	 
 */
typedef Object* (*ExternalValueFunction)(container_id id);

/**
 * Provides a container interface via values sourced externally.
 * To avoid overhead of a virtual function table and vptr which require storage for two addresses, 
 * a single function is used to retrieve.
 */
class ExternalValueContainer : Container {
	
	ExternalValueFunction externalValue;
	
public:
	ExternalValueContainer(ExternalValueFunction fn) {
		externalValue = fn;
	}
	
	/**
	 * Returns a transient value item. 
	 */
	Object* item(container_id id) {
		return externalValue(id);
	}
	
	void returnItem(Object* item) {
		delete item;
	}
	
	container_id size() {
		return container_id(pointer_scalar(externalValue(-1)));
	}
};
