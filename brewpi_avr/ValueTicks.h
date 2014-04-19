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
class ScaledTicksValue : public WritableValue
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
		writePlatformEndianBytes(&time, sizeof(time), out);
		writePlatformEndianBytes(&scale, sizeof(scale), out);
	}
	
	void writeMaskedFrom(DataIn& in, DataIn& mask) {
		// write the start and scale
		readPlatformEndianMaskedBytes(&start, sizeof(start), in, mask);
		readPlatformEndianMaskedBytes(&scale, sizeof(scale), in, mask);
	}
		
	/**
	 * Stream written/read is
	 *   4-bytes	uint32	The number of milliseconds passed since poweron.
	 *   2-bytes	uint16	The time scaling. a 16-bit integer. 
	 */
	uint8_t streamSize() { return 6; }
		
};

/**
 * Time is critical to so many components that this is provided as a system-level service.
 * The SystemProfile maintains this instance and persists changes to eeprom.
 */
extern ScaledTicksValue ticks;

/**
 * Fetches the current millisecond count from the {@code Ticks} static instance. 
 * TODO: now that ScaledTicksValue is part of the system container, we don't really need this?
 * It was mainly for testing, so can be replaced.
 */
class CurrentTicksValue : public Value
{

public:
	void readTo(DataOut& out)
	{
		ticks_millis_t millis = ticks.millis();
		writePlatformEndianBytes(&millis, 4, out);
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
	