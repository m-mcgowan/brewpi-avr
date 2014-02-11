#pragma once

/*
 * Commands.h
 *
 * Created: 05/02/2014 22:32:25
 *  Author: mat
 */ 

#include "DataStream.h"
#include "Values.h"
#include "ValuesEeprom.h"

typedef char* pchar;
typedef const char* cpchar;

void handleCommand(DataIn& data, DataOut& out);

void rehydrateObjects();

/**
 * Prototype for object factories. 
 */
typedef Object* (*ObjectFactory)(ObjectDefinition& def);

/**
 * Factory that consumes the object definition stream and returns {@code NULL}.
 */	
Object* nullFactory(ObjectDefinition& def);



enum Commands {
	CMD_NONE = 0,				// no-op

   	CMD_READ_VALUE = 1,			// read a value
   	CMD_WRITE_VALUE = 2,		// write a value
   	CMD_CREATE_OBJECT = 3,		// add object in a container
   	CMD_DELETE_OBJECT = 4,		// delete the object at the specified location
   	CMD_LIST_OBJECTS = 5,		// list objects in a container
	
	CMD_MAX = 127,				// max command value for user-visible commands
	CMD_SPECIAL_FLAG = 128,
	CMD_INVALID = CMD_SPECIAL_FLAG | CMD_NONE,						// special value for invalid command in eeprom. Used as a placeholder for incomplete data
	CMD_DISPOSED_OBJECT = CMD_CREATE_OBJECT | CMD_SPECIAL_FLAG	// flag in eeprom for object that is now deleted. Allows space to be reclaimed later.
};




class EepromStore {
public:	
	/**
	 * The eeprom stream. 
	 */
	static EepromDataOut writer;
	
	/**
	 * Initializes the eeprom. This formats the eeprom if not already done.
	 */
	static void initializeEeprom() {
            resetStream(writer);
	}
	
	static void formatEeeprom() {		
		for (int i=eepromAccess.length(); i-->0; ) {
			eepromAccess.writeByte(i, 0xFF);
		}
		
		writer.reset(0, eepromAccess.length());
		writer.write(0x96);		// marker
		writer.write(0x00);	
	}
	
	static void resetStream(EepromStreamRegion& region) {
		region.reset(0, eepromAccess.length());
	}
};
