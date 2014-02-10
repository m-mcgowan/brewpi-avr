/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
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

/*
 * This Atmel Studio 6 project automatically includes all needed Arduino source files, you just have to point it to the right directories.
 * To compile this project on your computer, you will have to set an environment variable to find your local Arduino installation.
 * Set the variable ARDUINO_HOME to point to your local Arduino path, without trailing slash, e.g. 'D:\arduino-1.01'. Instructions on the wiki here:
 * http://wiki.brewpi.com/index.php/Setting_up_the_brewpi-avr_Project
 * 'ArduinoFunctions.cpp' includes all the source files from Arduino that are used. You might have to edit it if you are not using a Leonardo.
 * That is all that is needed! No hassle with makefiles and compiling libraries.
 */
#include "Brewpi.h"
#include "Ticks.h"
#include "Comms.h"
#include "Commands.h"
#include "Values.h"
#include "ValuesEeprom.h"
#include "ValuesProgmem.h"
#include "GenericContainer.h"
#include "ValueModels.h"
#include "ValueController.h"

#if BREWPI_SIMULATE
	#include "Simulator.h"
#endif

// global class objects static and defined in class cpp and h files

// instantiate and configure the sensors, actuators and controllers we want to use


void setup(void);
void loop (void);

/* Configure the counter and delay timer. The actual type of these will vary depending upon the environment.
 * They are non-virtual to keep code size minimal, so typedefs and preprocessing are used to select the actual compile-time type used. */
TicksImpl ticks = TicksImpl(TICKS_IMPL_CONFIG);

StaticContainer<10> root;

Container* rootContainer()
{
	return &root;
}

struct Foo  {
	
	virtual void f()=0;
	virtual void g()=0;	
};

struct Bar {	
	virtual void h()=0;;
};

class Baz : public Foo, public Bar {	
	
};
	
class Quux : public Baz {
	virtual void g() {}
	virtual void h() {}		
	virtual void f() {}
	};

Quux q;

class BuildInfoValues : public FactoryContainer {
	private:	
	
	public:
	BuildInfoValues() {}
			
	container_id size() { return 1; }
	virtual Object* item(container_id id) { return new_object(ProgmemStringValue((PSTR(BUILD_NAME)))); }
};

// just experimenting - will do properly later
//BuildInfoValues buildInfo;
bool logValuesFlag = false;

class GlobalSettings {
	uint8_t settings[10];
		
	Object* externalValueHandler(container_id id) {
		if (id==-1) return (Object*)10;	// size
		if (id>9) return NULL;
		return new ExternalValue(settings+id, 1);
	}
};

void setup()
{    
	//root.add(root.next(), &buildInfo);
	//root.add(root.next(), &logInterval);
	
	Comms::init();
	
	// todo - safe mode - do not instantiate objects from eeprom
	
}

/*
 * Lifecycle for components:
 *
 * - prepare: general initialization, start of a new control loop
 * - update: fetch data from the environment, read sensor values
 * - log
 */
bool prepareCallback(Object* o, void* data, container_id* id) {
	
	uint32_t& waitUntil = *(uint32_t*)data;
	prepare_t millisToWait = o->prepare();
	if (millisToWait)
		waitUntil = ticks.millis()+millisToWait;
	return false;	// continue enumerating
}

/**
 * Updates each object in the container hierarchy.
 */
bool updateCallback(Object* o, void* data, container_id* id) {
	o->update();
	return false;
}

/**
 * Writes an ID chain to the stream.
 */
void writeID(container_id* id, DataOut& out) {
	do {
		out.write(*id);
	} while (*id++<0);
}

bool logValuesCallback(Object* o, void* data, container_id* id) {
	DataOut& out = *(DataOut*)data;
	if (isReadable(o)) {
		StreamReadable* r = (StreamReadable*)o;
		writeID(id, out);
		r->readTo(out);
	}
	return false;
}

/**
 * Logs all values in the system. 
 */
void logValues(container_id* ids)
{
	DataOut& out = Comms::dataOut();
	out.write(CMD_READ_VALUE);						
	walkRoot(rootContainer(), logValuesCallback, &out, ids);
	out.close();
}

void brewpiLoop(void)
{
	Comms::receive();

	container_id ids[MAX_CONTAINER_DEPTH];
	
	Container* root = rootContainer();


	uint32_t waitUntil = 0;	
	
	// todo - lifecycle methods should be delegated only to the container. The container itself then chooses if
	// it wants to delegate
	walkRoot(root, prepareCallback, &waitUntil, ids);
		
	for (;ticks.millis()<waitUntil;) { }
	
	walkRoot(root, updateCallback, NULL, ids);
	
	// todo - should brewpi always log, or only log when requested?
	if (logValuesFlag)
	{
		logValuesFlag = false;
		logValues(ids);
	}

}

void loop() {       
	#if 0 && BREWPI_SIMULATE
	simulateLoop();
	#else
	brewpiLoop();
	#endif
}

	
ObjectFactory createObjectHandlers[] = {
	nullFactory,
	OneWireBus::create,				
};

/**
 * The application supplied object factory.
 * Fetches the object type from the stream and looks this up against 
 */
Object* createObject(DataIn& in, bool dryRun=false)
{
	uint8_t type = in.next();		// object type
	if (dryRun || type>=sizeof(createObjectHandlers)/sizeof(createObjectHandlers[0]))
		type = 0;		// null object creator. Ensures stream is properly consumed even for invalid type values.
	
	uint8_t len = in.next();
	ObjectDefinition def(in, len, type);
	Object* result = createObjectHandlers[type](def);	
	return result;
}

