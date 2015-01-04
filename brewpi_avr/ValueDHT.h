#pragma once

#include "Values.h"
#include "dht.h"

/*
 * Exposes a DHT11/22 sensor as a pair of readable values.
 */
class ValueDHT : public Container
{
	dht _dht;
	uint8_t data;		// encoded data - bit 7 is set if this is a dht11 device, else dht21/22
						// bits 6-0 are the arduino pin. 
	ExternalValue value;
public:
	ValueDHT(): value(&_dht.humidity, 2) {}
	
	container_id size() { return 2; }
	
	Object* item(container_id id) {
		// This implementation hopes to save a bit of space by only having to instantiate
		// a single value. 		
		value.set(id ? &_dht.humidity : &_dht.temperature, sizeof(_dht.temperature));
		return &value;
	}
	
	prepare_t prepare() {
		_dht.prepare(data&0x7F);
		return data & 0x80 ? DHTLIB_DHT11_WAKEUP : DHTLIB_DHT22_WAKEUP;
	}
	
	/**
	 * Fetches the latest values from the DHT sensor.
	 */
	void update() {
		uint8_t pin = data&0x7F;
		if (data&0x80) {			
			_dht.read11(pin);
		}
		else {
			// readV21 and read22 are identical
			_dht.read22(pin);
		}
	}
			
	static Object* create(ObjectDefinition& def)
	{
		ValueDHT* result = new_object(ValueDHT());
		result->data = def.in->next();
		return result;
	}
};