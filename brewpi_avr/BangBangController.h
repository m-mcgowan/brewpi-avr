#pragma once

#include "Values.h"
#include "StreamUtil.h"

enum ControllerState
{
	idle = 0,
	heating = 1,
	cooling = 2	
};


class BangBangController : public Value
{
	
public:
	uint8_t state;
	uint8_t hysteresis;
	Value* sensor;
	Value* setpoint;
	Actuator* heater;
	Actuator* cooler;
	
	BangBangController(uint8_t _hysteresis, Value* _sensor, Value* _setpoint, Actuator* _heater, Actuator* _cooler) 
	: state(idle), hysteresis(_hysteresis), sensor(_sensor), setpoint(_setpoint), heater(_heater), cooler(_cooler) {}
	
	/**
	 * Output format:
	 * output state: 0
	 */
	void readTo(DataOut& out) {
		out.write(state);
	}	
		
	uint8_t streamSize() { return 1; }
		
	void update() {
		int16_t offset = read2BytesFrom(sensor) - read2BytesFrom(setpoint);		
		int16_t h = hysteresis;
		switch (state)
		{
			case idle: 
				state = (offset<-h) ? 2 : (offset>h) ? 1 : 0;
				break;
			case heating:
				if (offset>0)
					state = idle;
				break;
			case cooling:
				if (offset<0)
					state = idle;
		}
		updateActuators();
	}
	
	void updateActuators()
	{
		heater->setActive(state==heating);
		cooler->setActive(state==cooling);		
	}
		
	/**
	 * Configuration:
	 * hysteresis	(1 byte) - temperature in 1/16 of a degree. Up to 16C.
	 * id_chain - temp sensor id - read provides 2 bytes of temp data
	 * id_chain - setpoint object - provides 2 bytes for setpoint
	 * id_chain - cooler - cooler actuator
	 * id_chain - heater - heater actuator
	 * ? alarm temp (hi/low) - could be a separate object reading from the temp sensor
	 */
	static Object* create(ObjectDefinition& def) {
		BangBangController* result = new_object(BangBangController(
			def.in->next(), 
			(Value*)lookupUserObject(*def.in),
			(Value*)lookupUserObject(*def.in), 
			(Actuator*)lookupUserObject(*def.in), 
			(Actuator*)lookupUserObject(*def.in)));
		return result;
	}
};