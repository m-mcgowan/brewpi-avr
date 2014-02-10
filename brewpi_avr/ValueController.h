#pragma once
/*
 * ValueController.h
 *
 * Created: 10/02/2014 00:23:46
 *  Author: mat
 */ 

#include "Values.h"

#include "OneWire.h"

/**
 * A onewire bus is just an opaque object that is constructed and referenced. 
 */
// todo - I contemplated exposing e.g. pin as a value, but this is immutable, and is available from the object construction arguments
// mdma: I didn't push the Object functionality down into OneWire because that code comes from an external source - less intrusive to subclass.
class OneWireBus : public Object, public OneWire
{
	
public:	
	OneWireBus(uint8_t pin) : OneWire(pin) { }
	
	static Object* create(ObjectDefinition& def) {		
		uint8_t pin = def.in.next();
		return new_object(OneWireBus(pin));
	}
};