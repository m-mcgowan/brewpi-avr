/*
 * SystemProfile.cpp
 *
 * Created: 11/02/2014 03:33:11
 *  Author: mat
 */ 

#include "SystemProfile.h"
#include "Commands.h"

const uint8_t EEPROM_HEADER_SIZE = 2;
const uint8_t MAX_SYSTEM_PROFILES = 4;

const uint8_t SYSTEM_PROFILE_CURRENT_OFFSET = EEPROM_HEADER_SIZE;	// start address of profile fat.
const uint8_t SYSTEM_PROFILE_RESERVED_OFFSET = SYSTEM_PROFILE_CURRENT_OFFSET+1;
const uint8_t SYSTEM_PROFILE_OPEN_END = SYSTEM_PROFILE_RESERVED_OFFSET+1;
const uint8_t SYSTEM_PROFILE_FAT = SYSTEM_PROFILE_OPEN_END+sizeof(eptr_t);

const uint8_t SYSTEM_PROFILE_DATA_OFFSET = SYSTEM_PROFILE_FAT + (MAX_SYSTEM_PROFILES*sizeof(eptr_t));

typedef int8_t system_profile_t;

/**
 * Eeprom format:
 *    0x00	magic number (0x69)
 *	  0x01  version
 *	  0x02	active profile 
 *	  0x03	reserved
 *	  0x04  end of open profile 
 *	  0x06	start of profile FAT. 2 bytes per entry. This lists the start address of the profiles.
 *	  0x0E	start of profile storage
 *	  ....
 *	  end	
 */

inline eptr_t readPointer(eptr_t address) {
	eptr_t result;
	EepromDataIn in;
	in.reset(address, 2);
	in.read(&result, 2);
	return result;
}

inline void writePointer(eptr_t address, eptr_t v) {
	EepromDataOut out;
	out.reset(address, 2);
	out.writeBuffer((const void*)&v, stream_size_t(2));
}

void SystemProfile::initialize() {
	
	current = SYSTEM_PROFILE_DEFAULT;
	if (eepromAccess.readByte(0)==SYSTEM_PROFILE_MAGIC
		&& eepromAccess.readByte(1)==SYSTEM_PROFILE_VERSION) {
			// eeprom already initialized.			
	}
	else {
		writer.reset(0, eepromAccess.length());		
		writer.write(SYSTEM_PROFILE_MAGIC);			// marker
		writer.write(SYSTEM_PROFILE_VERSION);		// version
		writer.write(SYSTEM_PROFILE_NONE);			// no selected profile
		writer.write(0);							// reserved
		
		// clear the fat
		for (int i=SYSTEM_PROFILE_FAT; i<SYSTEM_PROFILE_DATA_OFFSET; i++)	{
			eepromAccess.writeByte(i, 0);
		}

		// reset profile store
		for (eptr_t i=SYSTEM_PROFILE_DATA_OFFSET; i<eepromAccess.length(); i++) {
			eepromAccess.writeByte(i, 0xFF);
		}
		
		// the end of the highest profile point (non-inclusive) is at the start
		setProfileOffset(-1, SYSTEM_PROFILE_DATA_OFFSET);	// next profile begins at the start
		
		// create first profile automatically and activate
		profile_id_t id = createProfile();
		activateProfile(id);		
	}	
}


/**
 * Creates a new profile. 
 * This enumerates the profile slots, looking for one that has the value 0, meaning unused, and then initializes
 * the slot to the location of the end of the previous profile.
 * The current profile is not changed, although if the current profile was previously open, it will be closed.
 */
profile_id_t SystemProfile::createProfile() {		
	eptr_t end = readPointer(SYSTEM_PROFILE_OPEN_END);	// find the end of the last profile
	
	// look for a free slot
	profile_id_t idx = -1;
	for (profile_id_t i=0; i<MAX_SYSTEM_PROFILES; i++) {		
		if (!getProfileOffset(i)) {
			idx = i; break;
		}
	}
	if (idx!=-1) {
		setProfileOffset(idx, end);
		resetStream(writer, false);		// reset the stream so that it's limited now.
	}	
	return idx;
}

/**
 * Activates the named profile. If the current profile is the same as the requested active profile, the method silently
 * returns. The new active profile ID is persisted to eeprom after successful activation.
 * @param profile	The profile id to activate. If this is -1, the current profile is deactivated and no profile is activated.
 */
bool SystemProfile::activateProfile(profile_id_t profile) {
	
	// todo - maybe simpler to write new profile to eeprom then reboot.
	// this will avoid fragmentation of the heap.		
	if (current!=profile) {		
		deactivateCurrentProfile();

		if (profile>=0) {
			current = profile;				// non-persistent change											
			EepromDataIn eepromReader;			
			resetStream(eepromReader);			// get region in eeprom for the profile
			BlackholeDataOut nullOut;
			PipeDataIn reader(eepromReader, nullOut);
			while (reader.hasNext()) {
				uint8_t cmd = reader.next();
				// if cmd is not create object then just parse the contents but don't instantiate. 
				rehydrateObject(eepromReader.offset(), reader, cmd!=CMD_CREATE_OBJECT);
				// todo - what to do with errors? ideally some logging might help				
			}
			resetStream(writer, true);		// reset to available region (allow open profile)
		}
		setCurrentProfile(profile);			// persist the change after profile instantiated.
	}
	return true;
}

/**
 * Deletes a profile. If the profile being deleted is the active profile, the current profile is deactivated.
 */
profile_id_t SystemProfile::deleteProfile(profile_id_t profile) {
	if (profile==current) {		// persistently unload profile if it's the one to be deleted
		activateProfile(-1);	
	}
	
	eptr_t start = getProfileOffset(profile);
	eptr_t end = getProfileEnd(profile);
	
	setProfileOffset(profile, 0);		// mark the slot as available
	for (int i=-1; i<MAX_SYSTEM_PROFILES; i++) {
		eptr_t e = getProfileOffset(i);
		if (e>=end) {
			setProfileOffset(i, e-(end-start));
		}
	}

	// block copy eeprom
	while (end<eepromAccess.length()) {
		uint8_t b = eepromAccess.readByte(end++);
		eepromAccess.writeByte(start++, b);
	}
	while (start<eepromAccess.length()) {
		eepromAccess.writeByte(start++, 0xFF);
	}
	return profile;
}

inline eptr_t profileFAT(profile_id_t id) {
	return SYSTEM_PROFILE_FAT+(id*2);
}

void SystemProfile::setProfileOffset(profile_id_t profile, eptr_t addr) {
	writePointer(profileFAT(profile), addr);
}
eptr_t SystemProfile::getProfileOffset(profile_id_t profile) {
	return readPointer(profileFAT(profile));
}


class BufferDataIn : public DataIn {
	const uint8_t* _data;
public:
	BufferDataIn(const void* data) : _data((const uint8_t*)data) {}
		
	uint8_t next() { return *_data++; }
	bool hasNext() { return true; }
	uint8_t peek() { return *_data; }	
};

/**
 * Deletes objects after any child objects have been deleted. Callback from container traversal. 
 */
bool deleteDynamicallyAllocatedObject(Object* obj, void* data, container_id* id, bool enter) {
	if (!enter && isDynamicallyAllocated(obj)) {		// delete on exit, so that all children are processed first
		BufferDataIn idStream(id);						// stream the id
		deleteObject(idStream);							// delete the object
	}
	return false;										// continue traversal
}

void SystemProfile::deactivateCurrentProfile() {
	if (current<0)
		return;
		
	// if this profile is open, store the offset in the writer stream
	if (getProfileEnd(current, true)==eepromAccess.length()) {
		setProfileOffset(-1, writer.offset());
	}
	
	container_id id[MAX_CONTAINER_DEPTH];				// buffer for id during traversal
	
	// delete all the objects that were dynamically allocated.
	walkRoot(deleteDynamicallyAllocatedObject, NULL, id);
	current = -1;		
	resetStream(writer);
}

void SystemProfile::setCurrentProfile(profile_id_t id) {
	current = id;
	eepromAccess.writeByte(SYSTEM_PROFILE_CURRENT_OFFSET, id);
}

/**
 * Retrieves the current profile.
 * @return the id of the current profile loaded. Negative if no profile loaded.
 */
profile_id_t SystemProfile::currentProfile() {
	return current;
}

void SystemProfile::resetStream(EepromStreamRegion& region, bool includeOpen) {	
	if (current>=0) {
		eptr_t offset = getProfileOffset(current);
		eptr_t end = getProfileEnd(current, includeOpen);
		region.reset(offset, end-offset);
	}
	else {
		region.reset(0,0);
	}
}

/**
 * Locates the end (exclusive) of the current profile.
 */
eptr_t SystemProfile::getProfileEnd(profile_id_t profile, bool includeOpen)  {
	eptr_t end = eepromAccess.length();
	eptr_t start = getProfileOffset(profile);
	
	// find smallest profile offset that is greater than the start
	for (profile_id_t i=-1; i<MAX_SYSTEM_PROFILES; i++) {		// include last profile end
		eptr_t p = getProfileOffset(i);
		if ((p>start) && (p<end) && (i>=0 || !includeOpen))
			end = p;
	}
	return end;
}

void SystemProfile::activateDefaultProfile() 
{	
	profile_id_t id = eepromAccess.readByte(SYSTEM_PROFILE_CURRENT_OFFSET);
	activateProfile(id);
}

EepromDataOut SystemProfile::writer;
profile_id_t SystemProfile::current;