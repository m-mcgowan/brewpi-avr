#pragma once
/*
 * Profile.h
 *
 * Created: 22/03/2014 17:31:05
 *  Author: mat
 */ 

#include "SystemProfile.h"
#include "Values.h"
#include "ValuesEeprom.h"
#include "Ticks.h"

#ifndef PROFILE_SMOOTHING
#define PROFILE_SMOOTHING 0
#endif

enum ProfileInterpolation {
	none,				// each profile step is constant until the next step
	linear,				// linear interpolation between the start and end of each step
	smooth,				// 
	smoother
};

struct ProfileState
{
	uint8_t		currentStep			:4;		// the current step in the profile
	uint8_t		reserved			:1;		// reserved (padding)
	uint8_t		running				:1;		// non-zero if this profile is running
	uint8_t 	interpolation		:2;		// the current interpolation type
	uint16_t 	currentTimeOffset;			// the current time offset within the current step
};

#define STATE_CURRENT_STEP_MASK 0xF0
#define STATE_INTERPOLATION_MASK 0x0C
#define STATE_RUNNING_MASK 0x02
#define STATE_RESERVED_MASK 0x01

#define STATE_INTERPOLATION_SHIFT 0
#define STATE_CURRENT_STEP_SHIFT 4

/**
 * A profile updates a 16-bit value in tandem with passing time. 
 * 
 */
class ProfileRaw : public EepromValue
{
	// I choose to make this static so that all profiles are kept on the same time, and also 
	// so that the code-side is smaller
	static ticks_millis_t lastTick;

	uint16_t calculateSetpoint(uint8_t step, uint8_t maxStep, uint16_t current);
	

	bool isRunning() {
		return state() & STATE_RUNNING_MASK;
	}
	
	ProfileInterpolation profileInterpolation() {
		return ProfileInterpolation((state() & STATE_INTERPOLATION_MASK) >> STATE_INTERPOLATION_SHIFT);
	}
	
	uint8_t currentStep() {
		return (state() & STATE_CURRENT_STEP_MASK) >> STATE_CURRENT_STEP_SHIFT;
	}
	
	void setCurrentStep(uint8_t step) {
		eepromAccess.writeByte(eeprom_offset(), (state()& ~STATE_CURRENT_STEP_MASK) | step);
	}
	
	uint8_t state() {
		return eepromAccess.readByte(eeprom_offset());
	}
	
	uint8_t stepCount() {
		return eepromAccess.readByte(eeprom_offset()+3);
	}
	
	uint16_t stepDuration(uint8_t step) {
		return readPointer(eeprom_offset()+4+(step<<1));
	}

	uint16_t stepSetpoint(uint8_t step) {
		return readPointer(eeprom_offset()+6+(step<<1));
	}

	uint16_t currentProfileTime() {
		return readPointer(eeprom_offset()+1);
	}
	
	void setCurrentProfileTime(uint16_t time) {
		writePointer(eeprom_offset()+1, time);
	}
	
	void setSetpoint(uint16_t setpoint);
	
public:
	// use default read to fetch profile state
	// default write is available but not recommended
	
	object_t objectType() {
		return otValueWrite|otNotLogged;
	}

	void writeMaskedFrom() {
		
	}

	void update();

	/**
	 *
	 */
	static Object* create(ObjectDefinition& defn) {
		// construction data shhould be 
		// 00 00 00 00 (initialize control block)
		// followed by uint16_t x 2 (big endian) for each profile point (duration and setpoint)
		// ID of target object to update (as on-wire encoding with 0x80 bit indicating more data to come.)
		
		
		return new ProfileRaw();
	}
	
	// TODO- can't decide if API should be a read/write block and let external code handle changes,
	// or if this object should be a container that exposes individual values (interpolation type, running, offset, time) as 
	// 
};
