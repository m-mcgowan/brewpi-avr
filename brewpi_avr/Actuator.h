/*
 * Copyright 2013 Matthew McGowan
 * Copyright 2013 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Brewpi.h"
#include "FastDigitalPin.h"
#include "Values.h"

/*
 * An actuator simply turns something on or off.                        
 */
// todo - actuators can be remodeled as a bool state-assignable value. See BasicReadWriteValue
class Actuator : public WritableValue
{
	public:	
	// todo - replace this with read()/write()
	virtual void setActive(bool active) =0;
	virtual bool isActive() =0;	
};

/*
 * An actuator that simply remembers the set value. This is primary used for testing.
 */
class ValueActuator : public Actuator
{
public:
	ValueActuator() : state(false) {}
	ValueActuator(bool initial) : state(initial) {}

	void setActive(bool active) { state = active; }
	bool isActive() { return state; }

	

private:
	bool state;	
};

