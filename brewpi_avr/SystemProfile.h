#pragma once

#include <stdint.h>
#include "Values.h"
#include "DataStreamEeprom.h"

typedef container_id profile_id_t;

/**
 * Internal value used to select the default profile. The default profile is the one that was most recently used.
 */
static const profile_id_t SYSTEM_PROFILE_DEFAULT = -2;

/**
 * Profile value used to indicate that no profile should be activated. 
 */
static const profile_id_t SYSTEM_PROFILE_NONE = -1;

const uint8_t SYSTEM_PROFILE_MAGIC = 0x69;
const uint8_t SYSTEM_PROFILE_VERSION = 0x01;

/**
 * SystemProfile - a special container used to provide access to system profiles.
 * A system profile is a set of object definitions.
 */
class SystemProfile {
	
	/**
	 * the selected profile.
	 */
	static profile_id_t current;
	
	static void setProfileOffset(profile_id_t id, eptr_t offset);
	static eptr_t getProfileOffset(profile_id_t id);
	static void setCurrentProfile(profile_id_t id);
		

	/**
	 * Deactivate the current profile by deleting all objects. (TODO: ideally this should be in reverse order, but I'm counting on objects not being
	 active during this time and that they have no resources to clean up.)
	 */
	static void deactivateCurrentProfile();

public:
	
	/**
	 * The eeprom stream that maintains the current write position in eeprom for the current profile.
	 */
	static EepromDataOut writer;

	
	
	/**
	 * Initialize this system profile handler.
	 */
	static void initialize();
	
	/*
	 * Load the profile last persisted. 
	 */
	static void activateDefaultProfile();
	
	/**
	 * Fetches the root container for the currently active profile.
	 * Even if no profile is active, still returns a valid root container with just the current profile
	 * value. 
	 */
	static Container* rootContainer();
		
	/**
	 * Create a new profile.
	 * @return the ID of the profile, or negative on error.
	 */
	static profile_id_t createProfile();
		
	/**
	 * deletes a profile. All profiles with indices larger than this are moved down to one index lower.
	 * All settings for the profile stored in persistent storage are removed and the space is freed up.
	 * If the current profile is the one being deleted, the profile is deactivated first. 
	 */
	static profile_id_t deleteProfile(profile_id_t profile);
	
	/**
	 * Activate the selected profile.
	 * @param The profile to activate. Can be -1 to deactivate the profile.
	 * The active profile is persistent.
	 */
	static bool activateProfile(profile_id_t index);
	
	
	/**
	 * Returns the id of the current profile, or -1 if no profile is active.
	 * @return The currently active profile index, or -1 if no profile is active.
	 */
	static profile_id_t currentProfile();

	static bool isCurrentProfileOpen();

	/**
	 * Resets the stream to the region in eeprom for the currently active profile. 
	 * If there is no profile, it is set to the end of eeprom, length 0.
	 */
	static void resetStream(EepromStreamRegion& region, bool includeOpen=false);

};


extern SystemProfile systemProfile;