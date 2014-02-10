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
 * The no-op command simply echoes the response until the end of stream.
 */
void noopCommandHandler(DataIn& _in, DataOut& out)
{	
	while (_in.hasNext());
}

/**
 * Implements the read value command. Accepts multiple ID chains and outputs each chain plus the
 * data for that object, or a 0-byte block if the object is not known or is not readable.
 */
void readValueCommandHandler(DataIn& in, DataOut& out) {	
	while (in.hasNext()) {					// allow multiple read commands
		Object* o = lookupObject(in);		// read the object and pipe read data to output
		uint8_t available = in.next();		// number of bytes available
		Value* v = (Value*)o;
		if (isValue(o) && available==v->streamSize()) {		
			v->readTo(out);
		}
		else {								// not a readable object, flag as 0 length
			while (available-->0) {			// consume data in stream
				in.next();
			}
			out.write(0);
		}
	}
}

// todo - factor common behaviour setValue/fetchValue functions - 

/**
 * Implements the set value command. 
 */
void setValueCommandHandler(DataIn& in, DataOut& out) {	
	while (in.hasNext()) {					// allow multiple ids
		Object* o = lookupObject(in);		// fetch the id
		
		Value* v = (Value*)o;			
		uint8_t available = in.next();
		if (isWritable(o) && v->streamSize()==available) {		// if it's writable and the correct number of bytes were parsed.
			v->writeFrom(in);									// assign from stream			
			out.write(v->streamSize());
			v->readTo(out);			
		}
		else {													// either not writable or invalid size
			while (available-->0) {								// consume rest of stream for this command
				in.next();
			}
			out.write(0);										// write 0 bytes (indicates failure)
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

Object* createEepromValue(ObjectDefinition& def) {	
	BasicReadWriteValue<eptr_t> offset(0);
	offset.writeFrom(*def.in);	
	EepromBaseValue* value = NULL;	
	value = new EepromDynamicStreamValue(offset.read(), def.len);
	return value;
}



/**
 * Consumes the definition data from the stream and returns a {@code NULL} pointer.
 */
Object* nullFactory(ObjectDefinition& def) {
	for (int i=def.len; i-->0; ) {
		def.in->next();
	}
	return NULL;
}

/**
 * Application-provided function to create the object of the given type.
 * @param in	The data input stream that provides the object details. The format is:
 * <pre>
 *	uint8_t			object type
 *  uint8_t			data length
 *  uint8_t[len]	data 
 * </pre>
 */
extern Object* createObject(DataIn& in, bool dryRun=false);


/**
 * The eeprom stream. 
 */
EepromDataOut eepromWriter(0, eepromAccess.length());


enum RehydrateErrors {
	rehydrateNoError = 0,
	rehydrateFail = 1			// descriptive :)
};

/**
 * Rehydrate an object from a definition. 
 * @param in	The stream containing the object definition. First the id, then the object type, definition block length and
 * the definition block.
 * @return 0 on success, an error code on failure.
 */
uint8_t rehydrateObject(eptr_t offset, DataIn& in) 
{	
	container_id lastID;
	Object* target = lookupContainer(in, lastID);			// find the container where the object will be added
	Object* newObject = createObject(in, false);			// read the type and create args
	
	uint8_t error = rehydrateFail;
	if (lastID>=0 && target && newObject && isOpenContainer(target)) {		// if the lastID >=0 then it was fetched from a container
		OpenContainer* c = (OpenContainer*)target;
		if (c->add(lastID, newObject)) {			
			newObject->rehydrated(offset);
			error = rehydrateNoError;
		}
	}
	
	if (error) {
		delete_object(newObject);
	}
	return error;
}


/**
 * Creates a new object at a specific location. 
 */
void createObjectCommandHandler(DataIn& _in, DataOut& out)
{
	PipeDataIn in(_in, eepromWriter);		// pipe object creation command to eeprom
	
	eptr_t offset = eepromWriter.offset();		// save current eeprom pointer - this is where the object definition is written.
	eepromWriter.write(CMD_INVALID);			// value for partial write, will go a back when successfully completed and write this again
	uint8_t error_code = rehydrateObject(offset, in);
	if (!error_code) {
		eepromAccess.writeByte(offset, CMD_CREATE_OBJECT);	// finalize creation in eeprom
	}
	out.write(error_code);							// status is index it was created at	
}


/**
 * Parses a stream of object definitions, piping the valid definitions to an output stream.
 * Stops when the input stream doesn't contain a recognized object definition. 
 */
class ObjectDefinitionWalker {
	
	DataIn* _in;		// using pointer to avoid non-POD warnings
	
public:
	ObjectDefinitionWalker(DataIn& in):
		_in(&in) {}
						
	/**
	 * Writes the next object definition from the data input to the given output.
	 * When the input stream is exhausted or the current position is not an object creation command,
	 * the method returns false and the stream location is unchanged. 
	 */							
	bool writeNext(DataOut& out) {
		if (!_in->hasNext())
			return false;
			
		int8_t next = _in->peek();
		bool valid =  ((next&0x7E)==CMD_CREATE_OBJECT);	// remove MSB and LSB so accepts CMD_CREATE_TL_OBJECT too
		if (valid) {			
			PipeDataIn pipe(*_in, next>=0 ? blackhole : out);	// next<0 if command not fully completed, so output is discarded
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

/*
 * Capture one command byte plus a streamed ID.
 */
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
void deleteObjectCommandHandler(DataIn& in, DataOut& out)
{
	IDCapture idCapture;						// buffer to capture id
	PipeDataIn id(in, idCapture);				// capture read id
	int8_t lastID;
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
	PipeDataIn pipeIn = PipeDataIn(dataIn, dataOut);	
	uint8_t next = pipeIn.next();
	handlers[next](pipeIn, dataOut);	
}

