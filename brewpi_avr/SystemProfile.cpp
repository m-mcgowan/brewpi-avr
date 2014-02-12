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

void SystemProfile::initialize() {
	
	if (eepromAccess.readByte(0)==SYSTEM_PROFILE_MAGIC
		&& eepromAccess.readByte(1)==SYSTEM_PROFILE_VERSION) {
			// eeprom already initialized.
			current = SYSTEM_PROFILE_DEFAULT;
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
				
		setProfileOffset(-1, SYSTEM_PROFILE_DATA_OFFSET);
		
		// create first profile by default.
		createProfile();
	}	
}

eptr_t readPointer(eptr_t address) {
	eptr_t result;
	EepromDataIn in;
	in.reset(address, 2);
	in.read(&result, 2);
	return result;
}

void writePointer(eptr_t address, eptr_t v) {
	EepromDataOut out;
	out.reset(address, 2);
	out.writeBuffer((const void*)&v, stream_size_t(2));
}

/**
 * Creates a new profile. 
 */
profile_id_t SystemProfile::createProfile() {
	deactivateCurrentProfile();
		
	eptr_t end = readPointer(SYSTEM_PROFILE_OPEN_END);
	// look for a free slot
	profile_id_t idx = -1;
	for (int i=SYSTEM_PROFILE_FAT; i<SYSTEM_PROFILE_DATA_OFFSET; i+=2) {
		if (!readPointer(i)) {
			idx = i; break;
		}
	}
	if (idx!=-1) {
		setProfileOffset(idx, end);
	}
	return idx;
}

profile_id_t SystemProfile::deleteProfile(profile_id_t profile) {
	if (profile==currentProfile()) {
		deactivateCurrentProfile();
	}
	// todo - need to fix up the end pointer if this is the profile that is at the end.
	// todo - need to compress eeprom storage space - block copy data above. 
	
	setProfileOffset(profile, 0);	
	return profile;
}

eptr_t profileFAT(profile_id_t id) {
	return SYSTEM_PROFILE_FAT+(id*2);
}

void SystemProfile::setProfileOffset(profile_id_t profile, eptr_t addr) {
	writePointer(profileFAT(profile), addr);
}



class BufferDataIn : public DataIn {
	const uint8_t* _data;
public:
	BufferDataIn(const void* data) : _data((const uint8_t*)data) {}
		
	uint8_t next() { return *_data++; }
	bool hasNext() { return true; }
	uint8_t peek() { return *_data; }	
};

bool deleteDynamicallyAllocatedObject(Object* obj, void* data, container_id* id, bool enter) {
	if (!enter && isDynamicallyAllocated(obj)) {		// delete on exit, so that all children are processed first
		BufferDataIn idStream(id);
		deleteObject(idStream);
	}
	return false;
}

void SystemProfile::deactivateCurrentProfile() {
	if (currentProfile()<0)
		return;
		
	container_id id[MAX_CONTAINER_DEPTH];
	
	// delete all the objects that were dynamically allocated.
	walkRoot(deleteDynamicallyAllocatedObject, NULL, id);	
}

profile_id_t SystemProfile::currentProfile() {
	return -1;
}

void SystemProfile::resetStream(EepromStreamRegion& region) {
	region.reset(SYSTEM_PROFILE_DATA_OFFSET, eepromAccess.length()-SYSTEM_PROFILE_DATA_OFFSET);
}

void SystemProfile::activateDefaultProfile() {	
}

EepromDataOut SystemProfile::writer;
profile_id_t SystemProfile::current;