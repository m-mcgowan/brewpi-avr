#pragma once

#include "Ticks.h"

/**
 * Fetches the current millisecond count from the {@code Ticks} static instance. 
 */
class CurrentTicksValue : public Value
{

public:
	void readTo(DataOut& out)
	{
		ticks_millis_t millis = ticks.millis();
#if ARDUINO				
		// arduino is little-endian
		uint8_t* buf = (uint8_t*)&millis;				
		for (int i=4; i-->0; )
			out.write(buf[i]);
#else		
		// intel is big endian
		out.writeBuffer(&millis, sizeof(millis));
#endif		
	}
	
	uint8_t streamSize()
	{
		return sizeof(ticks_millis_t);
	}
	
	static Object* create(ObjectDefinition& def) {
		// no parameters required
		return new_object(CurrentTicksValue());
	}
};

/**
 * Remembers the time at the start of the current cycle. 
 */
class CurrentCycleTicksValue : public CurrentTicksValue
{
	ticks_millis_t cycle_ticks;
public:	

	void update() {
		cycle_ticks = ticks.millis();
	}

	void readTo(DataOut& out) {
		out.writeBuffer(&cycle_ticks, sizeof(cycle_ticks));
	}
};
	