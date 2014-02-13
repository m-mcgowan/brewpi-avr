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

/**
 * @param	offset	The location in eeprom of this object
 * @param	in		Commands for the object, starting after the command id.
 * @return	>=0 on success, <0 on error.
 */
uint8_t rehydrateObject(eptr_t offset, PipeDataIn& in, bool dryRun=false);

void rehydrateObjects();

/**
 * Delete an object (but not the definition in eeprom.)
 */
uint8_t deleteObject(DataIn& id);

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



