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

const uint16_t SYSTEM_PROFILE_EEPROM_HEADER = 		uint16_t(SYSTEM_PROFILE_MAGIC)<<8 | SYSTEM_PROFILE_VERSION;

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

eptr_t readPointer(eptr_t address) {
	eptr_t result;
	uint8_t* d = (uint8_t*)&result;
	*d++ = eepromAccess.readByte(address);
	*d = eepromAccess.readByte(++address);
	return result;
}

void writePointer(eptr_t address, eptr_t v) {
	eepromAccess.writeByte(address, v>>8);
	eepromAccess.writeByte(address+1, v&0xFF);
}

inline void writeEepromRange(eptr_t start, eptr_t end, uint8_t data) {
	while (start<end) {
		eepromAccess.writeByte(start++, data);
	}
}

void SystemProfile::initialize() {
	
	current = SYSTEM_PROFILE_DEFAULT;
	if (readPointer(0)==SYSTEM_PROFILE_EEPROM_HEADER) {
			// eeprom already initialized.			
	}
	else {
		writePointer(0, SYSTEM_PROFILE_EEPROM_HEADER);
		writePointer(2, -1);
		
		// clear the fat
		writeEepromRange(SYSTEM_PROFILE_FAT, SYSTEM_PROFILE_DATA_OFFSET, 0);
		
		// reset profile store
		writeEepromRange(SYSTEM_PROFILE_DATA_OFFSET, eepromAccess.length(), 0xFF);
		
#if SYSTEM_PROFILE_ENABLE		
		// the end of the highest profile point (non-inclusive) is at the start
		setProfileOffset(-1, SYSTEM_PROFILE_DATA_OFFSET);	// next profile begins at the start
		profile_id_t id = createProfile();
#else
		profile_id_t id = 0;		
#endif		
		// create first profile automatically and activate
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

	profile_id_t idx = -1;

#if SYSTEM_PROFILE_ENABLE
	eptr_t end = readPointer(SYSTEM_PROFILE_OPEN_END);	// find the end of the last profile
	
	// look for a free slot
	for (profile_id_t i=0; i<MAX_SYSTEM_PROFILES; i++) {		
		if (!getProfileOffset(i)) {
			idx = i; break;
		}
	}
	
	if (idx!=-1) {
		setProfileOffset(idx, end);
		profileWriteRegion(writer, false);		// reset the stream so that it's limited now.
	}	
#endif
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
#if SYSTEM_PROFILE_ENABLE	
		deactivateCurrentProfile();
#endif		

		if (profile>=0) {
			current = profile;				// non-persistent change											
			EepromDataIn eepromReader;			
			profileReadRegion(eepromReader);			// get region in eeprom for the profile
			BlackholeDataOut nullOut;
			PipeDataIn reader(eepromReader, nullOut);	// rehydrateObject expects a pipe stream to save the object definition. we just throw it away.
			while (reader.hasNext()) {
				uint8_t cmd = reader.next();
				// if cmd is not create object then just parse the contents but don't instantiate. 
				rehydrateObject(eepromReader.offset(), reader, cmd!=CMD_CREATE_OBJECT);
				// todo - what to do with errors? at least log errors. 
			}
			// TODO this is wrong - stream should be reset from the profile end to eepromAccess.length() if this is open
			// else set to end of profile, length 0.
			profileWriteRegion(writer, true);		// reset to available region (allow open profile)
		}
		setCurrentProfile(profile);			// persist the change after profile instantiated.
	}
	return true;
}

/**
 * Deletes a profile. If the profile being deleted is the active profile, the current profile is deactivated.
 */
profile_id_t SystemProfile::deleteProfile(profile_id_t profile) {
#if SYSTEM_PROFILE_ENABLE
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
	writeEepromRange(start, eepromAccess.length(), 0xFF);		
	return profile;
#else
	return -1;	// not supported	
#endif	
}

#if SYSTEM_PROFILE_ENABLE
eptr_t profileFAT(profile_id_t id) {
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

/**
 * Deactivates the current profile. If there is no profile currently active this is a no-op.
 * Deactivation is transient - the eeprom still contains the previously active profile.
 */
void SystemProfile::deactivateCurrentProfile() {
	if (current<0)
		return;
		
	// if this profile is open, be sure to compact eeprom
	if (getProfileEnd(current, true)==eepromAccess.length()) {
		eptr_t end = compactObjectDefinitions();
		setProfileOffset(-1, end);
	}
	
	container_id id[MAX_CONTAINER_DEPTH];				// buffer for id during traversal
	
	// delete all the objects that were dynamically allocated.
	walkRoot(deleteDynamicallyAllocatedObject, NULL, id);
	current = -1;		
	profileWriteRegion(writer);
}
#endif

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

/**
 * Sets the stream region to be the writable portion of a profile storage.
 * @param region	The region to set
 * @param includeOpen	If the current profile is the open one. The end is then set to the end of eeprom. 
 */
void SystemProfile::profileWriteRegion(EepromStreamRegion& region, bool includeOpen) {	
	if (current>=0) {
		eptr_t offset = getProfileEnd(current);
		eptr_t end = getProfileEnd(current, includeOpen);
		region.reset(offset, end-offset);
	}
	else {
		region.reset(0,0);
	}
}

/**
 * Sets the stream to correspond with the start and end locations for the current profile.
 * 
 */
void SystemProfile::profileReadRegion(EepromStreamRegion& region) {
	if (current>=0) {
		eptr_t offset = getProfileOffset(current);
		eptr_t end = getProfileEnd(current, false);
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
		if ((p>start) && (p<end) && (i>=0 || !includeOpen))		// when i==-1 and !includeOpen then the end is used, otherwise end remains at eepromAccess.length()
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