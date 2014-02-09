/*
 * Commands.cpp
 *
 * Created: 05/02/2014 22:35:10
 *  Author: mat
 */ 

#include "Brewpi.h"
#include "Values.h"
#include "Commands.h"
#include "DataStream.h"
#include "ValuesEeprom.h"
#include "GenericContainer.h"

/**
 * equivalent to /dev/null. Used for discarding output.
 */
BlackholeDataOut blackhole;

/**
 * A command handler function. This is the signature of commands.
 * @param in	The data stream providing input to the command.
 * @param out	The data stream that accepts output from the command.
 */
typedef void (*CommandHandler)(DataIn& in, DataOut& out);

/**
 * Fetches the object at a given index in a container.
 * @param o	The object containing the object to fetch. This may be NULL and may or may not be a container.
 * @param id	The id to fetch. May include the 0x80 flag for on the wire encoding that this is not the last
 *   id in the chain.
 * @return The fetched object, or {@code NULL} if the object could not be fetched. 
 */
Object* fetchContainedObject(Object* o, uint8_t id)
{
	Object* result = NULL;
	id &= 0x7F;
	if (isContainer(o))
	{
		Container* c = (Container*)o;
		if (id<c->size())
			result = c->item(id);
	}
	else {
		// special case of 0 is also allowed as a self reference for non-container objects
		// this allows ids to be padded with 0 bytes without affecting the lookup		
		if (!id)		
			result = o;
	}
	return result;
}

/**
 * Lookup the object fetching the id chain from a stream.
 * @param data	The data stream to read the id chain.
 * @return The fetched object, or {@code NULL} if the id in the stream doesn't correspond to 
 */
Object* lookupObject(DataIn& data) {	
	Object* current = rootContainer();
	int8_t id = -1;
	while (data.hasNext() && id<0)
	{
		id = int8_t(data.next());							// msb set if there is more bytes in the id.
		current = fetchContainedObject(current, uint8_t(id));		
	}
	return current;	
}

// todo - factor lookupObject/lookupContainer

/**
 * Fetches the container and last id that corresponds to the id chain read from the data stream.
 * @param data	The stream providing the id chain of the object to lokup
 * @param lastID	[outval] Receives the last part of the id chain.
 *
 * For example, given a stream encoding the id chain "2.3.5", the container returned would correspond with
 * object "2.3" and lastID would be set to 5.
 */
Object* lookupContainer(DataIn& data, int8_t& lastID) 
{
	Object* current = rootContainer();
	int8_t id = int8_t(data.next());
	while (id<0 && data.hasNext())
	{
		current = fetchContainedObject(current, uint8_t(id));
		id = int8_t(data.next());
	}
	lastID = id;
	return current;
}


void noopCommandHandler(DataIn& _in, DataOut& out)
{	
}

/**
 * Implements the read value command. Accepts multiple ID chains and outputs each chain plus the
 * data for that object, or a 0-byte block if the object is not known or is not readable.
 */
void readValueCommandHandler(DataIn& in, DataOut& out) {
	PipeDataIn pipe(in,out);
	while (in.hasNext()) {					// allow multiple read commands
		Object* o = lookupObject(pipe);		// read the object and pipe read data to output
		if (isReadable(o)) {
			StreamReadable* r = (StreamReadable*)o;
			out.write(r->streamSize());
			r->readTo(out);
		}
		else {								// not a readable object, flag as 0 length
			out.write(0);
		}
	}
}

// todo - factor common behaviour setValue/fetchValue functions - 

/**
 * Implements the set value command. 
 */
void setValueCommandHandler(DataIn& in, DataOut& out) {
	PipeDataIn pipe(in,out);
	while (in.hasNext()) {					// allow multiple ids
		Object* o = lookupObject(pipe);		// fetch the id and pipe the id to output
		if (isWritable(o)) {				
			StreamWritable* w = (StreamWritable*)o;
			uint8_t available = in.next();
			w->writeFrom(in);
			out.write(available);

			StreamReadable* r = (StreamReadable*)o;
			out.write(r->streamSize());
			r->readTo(out);
			// todo - pass length of datablock to writeFrom so it can be validated
		}
		else {
			out.write(0);		// not writable or not known
		}
	}
}

enum ObjectTypes {
	OBJ_VALUE = 1,				// BasicValue
	OBJ_EEPROM_VALUE = 2		// EepromValue (first byte is length of value)	
};

/*
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
*/

Object* createEepromValue(DataIn& in, bool dryRun) {
	uint8_t len = in.next();
	BasicReadWriteValue<eptr_t> offset(0);
	offset.writeFrom(in);	
	EepromBaseValue* value = NULL;
	if (!dryRun)
		value = new EepromDynamicStreamValue(offset.read(), len);
	return value;
}


typedef Object* (*CreateObjectHandler)(DataIn& in, bool dryRun);

// todo - this should not be here, but part of the host application that defines the type of objects available

CreateObjectHandler createObjectHandlers[] = {
	NULL, //createBasicValue,
	createEepromValue		// todo - this is more for illustration than actual necessity
};

Object* createObject(DataIn& in, bool dryRun=false)
{	
	uint8_t type = in.next();		// object type
	if (type>=sizeof(createObjectHandlers)/sizeof(createObjectHandlers[0]))
		return NULL;
		
	return createObjectHandlers[type](in, dryRun);
}

// todo - initialize eeprom 
/**
 * The eeprom stream. 
 */
EepromDataOut eepromWriter(0, eepromAccess.length());


/**
 * Creates a new object at a specific location. 
 */
void createObjectCommandHandler(DataIn& _in, DataOut& out)
{
	PipeDataIn result(_in, out);			// pipe what is read back as result status
	PipeDataIn in(result, eepromWriter);	// pipe also to eeprom
	
	eptr_t offset = eepromWriter.offset();	// save current eeprom pointer
	eepromWriter.write(CMD_INVALID);		// value for partial write, will go a back when successfully completed and write this again
	
	container_id lastID;
	Object* target = lookupContainer(in, lastID);			// find the container where the object will be added
	Object* newObject = createObject(in);		// read the type and create args
	
	int8_t index = -1;
	if (lastID>=0 && target && newObject && isOpenContainer(target)) {		// if the lastID >=0 then it was fetched from a container
		OpenContainer* c = (OpenContainer*)target;
		bool success = c->add(lastID, newObject);
		// TODO - what is stored in eeprom needs to be absolute, i.e. turned from createObject to placeObject.
		// so we can identify it by the ID. Maybe just drop this command and have a command to fetch the next
		// empty slot in a container.
		if (success)
			eepromAccess.writeByte(offset, CMD_CREATE_OBJECT);	// finalize creation in eeprom
	}
	else {
		delete newObject;
	}
	out.write(index);						// status is index it was created at
	
}


/**
 * 
 */
class ObjectDefinitionWalker {
	
	DataIn& _in;
	
public:
	ObjectDefinitionWalker(DataIn& in):
		_in(in) {}
						
	/**
	 * Writes the next object definition from the data input to the given output.
	 * When the input stream is exhausted or the current position is not an object creation command,
	 * the method returns false and the stream location is unchanged. 
	 */							
	bool writeNext(DataOut& out) {
		if (!_in.hasNext())
			return false;
			
		int8_t next = _in.peek();
		bool valid =  ((next&0x7E)==CMD_CREATE_OBJECT);	// remove MSB and LSB so accepts CMD_CREATE_TL_OBJECT too
		if (valid) {			
			PipeDataIn pipe(_in, next>=0 ? blackhole : out);	// next<0 if command not fully completed, so output is discarded
			pipe.next();										// fetch the next value already peek'ed at so this is written to the output stream
			/*Object* target = */lookupObject(pipe);			// find the container where the object will be added
			// todo - could flag warning if target is NULL
			createObject(pipe, true);							// dry run for create object, just want data to be output
		}
		return valid;
	}
};

template <int SIZE> class BufferDataOut : public DataOut {
	uint8_t buffer[SIZE];
	uint8_t pos;
public:
	BufferDataOut<SIZE>() { reset(); }
	
	void write(uint8_t data) {
		if (pos<SIZE) {
			buffer[pos++] = data;
		}
	}
	
	void reset() {
		pos = 0;
	}
	
	uint8_t size() {
		return pos;
	}
	
	const uint8_t* data() {
		return buffer;
	}
};

typedef BufferDataOut<MAX_CONTAINER_DEPTH+1> IDCapture;

/**
 * Finds the corresponding create object command in eeprom and marks it as invalid. The command is then ignored,
 * and will be removed from eeprom next time eeprom is compacted. 
 */
void removeEepromCreateCommand(IDCapture& id) {
	EepromDataIn eepromData(0, eepromAccess.length());
	ObjectDefinitionWalker walker(eepromData);
	IDCapture capture;							// save the contents of the eeprom
	
	eptr_t offset = eepromData.offset();		// save the offset at the start of the creation block
	while (walker.writeNext(capture)) {			// parse creation data. The first part (command + object id) is stored in the capture buffer.
		if (capture.size()) {					// valid entry written
			// compare captured id with the id we are looking for
			if (!memcmp(capture.data()+1, id.data(), id.size())) {
				eepromAccess.writeByte(offset, CMD_DISPOSED_OBJECT);				
			}			
			capture.reset();
		}
		offset = eepromData.offset();			
	}
}

/**
 * Handles the delete object command.
 *
 */
void deleteObjectCommandHandler(DataIn& _in, DataOut& out)
{
	int8_t lastID;
	PipeDataIn in(_in, out);			// pipe read input to output
	IDCapture idCapture;				// capture id
	PipeDataIn id(in, idCapture);		// capture read id
	Object* obj = lookupContainer(id, lastID);	// find the container and the ID in the chain to remove	
	uint8_t success = 0;
	if (obj!=NULL && isOpenContainer(obj) && lastID>=0) {
		OpenContainer* c = (OpenContainer*)obj;
		c->remove(lastID);
		success = 1;
	}	

	removeEepromCreateCommand(idCapture);	
	out.write(success);	
}




/**
 * Enumerates all the create object instructions in eeprom to an output stream.
 */ 
void listEepromInstructionsTo(DataOut& out) {
	EepromDataIn eepromData(0, eepromAccess.length());
	ObjectDefinitionWalker walker(eepromData);
	while (walker.writeNext(out));
}

/**
 * Compacts the eeprom instruction store by removing deleted object definitions.
 *
 * @return The offset where the next eeprom instruction will be stored. 
 */
eptr_t compactObjectDefinitions() {
	EepromDataOut eepromData(0, eepromAccess.length());
	listEepromInstructionsTo(eepromData);
	return eepromData.offset();
}

/**
 * Walks the eeprom and writes out the construction definitions.
 */
void listObjectsCommandHandler(DataIn& _in, DataOut& out)
{
	listEepromInstructionsTo(out);
}

CommandHandler handlers[] = {
	noopCommandHandler,
	readValueCommandHandler,
	setValueCommandHandler,	
	createObjectCommandHandler,	
	deleteObjectCommandHandler,
	listObjectsCommandHandler
};

/*
 * Processes the command request from a data stream.
 * @param dataIn The request data. The first byte is the command id. The stream is assumed to contain at least
 *   this data. 
 */
void handleCommand(DataIn& dataIn, DataOut& dataOut)
{
	uint8_t next = dataIn.next();
	dataOut.write(next);
	handlers[next](dataIn, dataOut);	
}

