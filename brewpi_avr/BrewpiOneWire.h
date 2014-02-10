#pragma once

/**
 * Normally objects go in their own header file if they are created as part of this project.
 * This file is for brewpi objects that are based on external code, so we don't want to modify the base object definition.
 */

#include "Values.h"

#include "OneWire.h"

/**
 * A onewire bus is just an opaque object that is constructed and referenced. 
 */
// todo - I contemplated exposing e.g. pin as a value, but this is immutable, and is available from the object construction arguments
// mdma: I didn't push the Object functionality down into OneWire because that code comes from an external source - less intrusive to subclass.
class OneWireBus : public Value, public OneWire
{
	
public:	
	OneWireBus(uint8_t pin) : OneWire(pin) { }
	
	static Object* create(ObjectDefinition& def) {		
		uint8_t pin = def.in->next();
		return new_object(OneWireBus(pin));
	}
	
	/**
	 * Enumerates the one-wire bus and returns the devices found.
	 */
	void readTo(DataOut& out) {
		reset_search();
		uint8_t address[8];
		while (search(address)) {
			// hardware device type from OneWire family ID
			out.write(address, 8);
		}
		out.write(0);
	}
	
	uint8_t streamSize() {
		// todo - what do to when there are more than 32 devices on the bus?
		// todo - don't really want to have to determine the stream size by enumerating devices to find out how
		// many there are. Make 255 mean unknown size, and that the caller has to use a device-specific enumeration.		
		return 255;
	}
	
};

