/*
 * Commands.cpp
 *
 * Created: 05/02/2014 22:35:10
 *  Author: mat
 */ 

#include "Brewpi.h"
#include "Values.h"
#include "Commands.h"
#include "ValuesEeprom.h"

typedef char* pchar;
typedef const char* cpchar;

const bool RANGE_CHECK = true;

class BuildInfoValues : public Container {
	private:
	BasicValue<cpchar> buildHash;
	
	public:
	BuildInfoValues()
	: buildHash(BUILD_NAME)	{}
	
	container_id size() { return 1; }
	void* item(container_id id) { return &buildHash; }
};


class RootContainer : public Container {
	BuildInfoValues buildInfo;
		
	container_id size() { return 1; }
	
	void* item(container_id id) { return &buildInfo; }
	
};

RootContainer root;

typedef void (*CommandHandler)(DataIn&, DataOut&);

Container* rootContainer()
{
	return &root;		
}

bool isContainer(Object* o) 
{
	return o!=NULL && o->objectType()==otContainer;
}

Object* fetchContainedObject(Object* o, uint8_t id)
{
	Object* result = NULL;
	if (isContainer(o))
	{
		Container* c = (Container*)o;
		if (RANGE_CHECK && id<c->size())
			result = c->item(id & 0x7F);
	}
	else {
		// special case of 0 is also allowed as a self reference for non-container objects
		// this allows ids to be padded with 0 bytes without affecting the lookup		
		if (!id)		
			result = o;
	}
	return result;
}

Object* lookupObject(DataIn& data) {	
	Object* current = rootContainer();
	int8_t id;
	do
	{
		id = int8_t(data.next());							// msb set if there is more bytes in the id.
		current = fetchContainedObject(current, uint8_t(id));		
	} while (id>=0);
	return current;	
}

// todo - factor lookupObject/lookupContainer
Object* lookupContainer(DataIn& data, int8_t& lastID) 
{
	Object* current = rootContainer();
	int8_t id = int8_t(data.next());
	while (id<0)
	{
		current = fetchContainedObject(current, uint8_t(id));
		id = int8_t(data.next());
	}
	lastID = id;
	return current;
}



bool isReadable(Object* o)
{
	return o->objectType()==otValue;
}

boolean isWritable(Object* o)
{
	return o->objectType()==otValue && (o->objectType()&otWritable);
}

/*
 * Reads data from a DataIn, and writes any fetched bytes to DataOut.
 */
class PipeDataIn : public DataIn
{
	DataIn& in; 
	DataOut& out;

public:	
	PipeDataIn(DataIn& in, DataOut& out) 
		: in(in), out(out) 
	{		
	}
	
	virtual uint8_t next() {
		uint8_t val = in.next();
		out.write(val);
		return val;
	}
	
	virtual bool hasNext() { return in.hasNext(); }
	virtual uint8_t peek() { return in.peek(); }
	
};


enum Commands {
	CMD_NONE = 0,				// no-op

	CMD_READ_VALUE = 1,			// read a value
	CMD_WRITE_VALUE = 2,			// write a value
	CMD_CREATE_OBJECT = 3,		// add object in a container
	CMD_PLACE_OBJECT = 4,		// create object and place a container at a specified offset
	CMD_DELETE_OBJECT = 5,		// delete the object at the specified location
	
	CMD_MAX = 127,				// max command value for user-visible commands
	CMD_INVALID = 128,			// special value for invalid command in eeprom. Used as a placeholder for incomplete data
	CMD_DISPOSED_OBJECT = 3+128	// flag in eeprom for object that is now deleted. Allows space to be reclaimed later.
};


void noopCommandHandler(DataIn& _in, DataOut& out)
{	
}

void fetchValueCommandHandler(DataIn& in, DataOut& out) {
	PipeDataIn pipe(in,out);
	while (in.hasNext()) {	// allow multiple read commands
		Object* o = lookupObject(pipe);
		if (isReadable(o)) {
			StreamReadable* r = (StreamReadable*)o;
			r->readTo(out);
		}		
	}
}

// todo - factor common behaviour setValue/fetchValue functions - 
void setValueCommandHandler(DataIn& in, DataOut& out) {
	PipeDataIn pipe(in,out);
	while (in.hasNext()) {	// allow multiple read commands
		Object* o = lookupObject(in);
		if (isWritable(o)) {
			StreamWritable* w = (StreamWritable*)o;
			w->writeFrom(in);
		}		
	}
}

enum ObjectTypes {
	OBJ_VALUE = 1,				// BasicValue
	OBJ_EEPROM_VALUE = 2		// EepromValue (first byte is length of value)
	
};

Object* createBasicValue(DataIn& in) 
{
	uint8_t len = in.next();
	
	// todo - should we have a dynamic size value?
	AbstractStreamValue* v = NULL;
	switch (len) {
		case 1:
			v = new BasicValue<uint8_t>(0);
			break;
		case 2:
			v = new BasicValue<uint16_t>(0);
			break;
		case 4:
			v = new BasicValue<uint32_t>(0);
			break;
	}
	
	if (v)
		v->writeFrom(in);	
	return v;
}

Object* createEepromValue(DataIn& in) {
	uint8_t len = in.next();
	BasicValue<eptr_t> offset(0);
	offset.writeFrom(in);	
	EepromBaseValue* value = new EepromDynamicValue(offset.read(), len);
	return value;
}


typedef Object* (*CreateObjectHandler)(DataIn& in);

// todo - this should not be here, but part of the host application that defines the type of objects available

CreateObjectHandler createObjectHandlers[] = {
	createBasicValue,
	createEepromValue		// todo - this is more for illustration than actual necessity
};

Object* createObject(DataIn& in)
{	
	uint8_t type = in.next();		// object type
	return NULL;
}

EepromDataOut eepromWriter(0, eepromAccess.length());

void createObjectCommandHandler(DataIn& _in, DataOut& out)
{
	// save current eeprom pointer
	// 
	
	eptr_t offset = eepromWriter.offset();	
	PipeDataIn result(_in, out);			// pipe what is read back as result status	
	PipeDataIn in(result, eepromWriter);			// pipe also to eeprom
		
	eepromWriter.write(CMD_INVALID);	// value for partial write, will go a back when succesfully completed and write this again
		
	Object* target = lookupObject(in);			// find the container where the object will be added	
	Object* newObject = createObject(in);
	
	int8_t index = -1;
	if (target && newObject && isContainer(target)) {		
		Container* c = (Container*)target;
		index = c->add(newObject);
		if (index>=0)				
			eepromAccess.writeByte(offset, CMD_CREATE_OBJECT);	// finalize creation in eeprom
	}
	else 
		delete newObject;
	out.write(index);						// status is index it was created at
}

void placeObjectCommandHandler(DataIn& _in, DataOut& out)
{
	out.write(0);
}



void deleteObjectCommandHandler(DataIn& _in, DataOut& out)
{
	int8_t lastID;
	PipeDataIn in(_in, out);
	Object* obj = lookupContainer(in, lastID);
	Object* removed = NULL;
	if (obj!=NULL && isContainer(obj) && lastID>=0) {
		Container* c = (Container*)obj;
		removed = c->remove(lastID);
		delete removed;
	}	
	
	// todo - locate object definition in eeprom and flag the object as deleted
	out.write(removed ? 1 : 0);	
}

CommandHandler handlers[] = {
	noopCommandHandler,
	fetchValueCommandHandler,
	setValueCommandHandler,
	createObjectCommandHandler,
	placeObjectCommandHandler,
	deleteObjectCommandHandler
};

void handleCommand(DataIn& dataIn, DataOut& dataOut)
{
	uint8_t next = dataIn.next();
	handlers[next](dataIn, dataOut);
}