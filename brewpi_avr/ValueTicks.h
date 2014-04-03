#pragma once

#include "Ticks.h"
#include "StreamUtil.h"

/*
 * Allows time to run normally, or allows external code to manipulate time for testing and simulation.
 * The original ticks instance has been renamed baseticks, and this provides the current (real) time
 * for the platform, which is used by this class to compute the scaled time.
 * 
 * The current time and scale are not persisted to eeprom.
 */
class ScaledTicksValue : public Value
{
	ticks_millis_t start;
	uint16_t scale;
	
public:	
	ScaledTicksValue() : start(0), scale(1) {}

	ticks_millis_t millis() {
		uint32_t real_now = baseticks.millis()-start;
		return start + (real_now*scale);
	}
	
	ticks_seconds_t seconds() { return millis()/1000; }

		
	void readTo(DataOut& out) {
		ticks_millis_t time = millis();
		writeBytes(&time, sizeof(time), out);
		writeBytes(&scale, sizeof(scale), out);
	}
	
	void writeMaskedFrom(DataIn& in, DataIn& mask) {
		// write the start and scale
		readMaskedBytes(&start, sizeof(start), in, mask);
		readMaskedBytes(&scale, sizeof(scale), in, mask);
	}
		
	uint8_t streamSize() { return 6; }
		
};

extern ScaledTicksValue ticks;

/**
 * Fetches the current millisecond count from the {@code Ticks} static instance. 
 */
class CurrentTicksValue : public Value
{

public:
	void readTo(DataOut& out)
	{
		ticks_millis_t millis = ticks.millis();
		writeBytes(&millis, 4, out);
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
	