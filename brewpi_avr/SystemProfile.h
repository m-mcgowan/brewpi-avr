#pragma once

#include <stdint.h>
#include "Values.h"

typedef container_id profile_id_t;

/**
 * Internal value used to select the default profile. The default profile is the one that was most recently used.
 */
static const profile_id_t SYSTEM_PROFILE_DEFAULT = -2;

/**
 * Profile value used to indicate that no profile should be activated. 
 */
static const profile_id_t SYSTEM_PROFILE_NONE = -1;

/**
 * SystemProfile - a special container used to provide access to system profiles.
 * A system profile is a set of object definitions.
 */
class SystemProfile : public OpenContainer {
	profile_id_t current;
	
public:

	SystemProfile() {
		// assign the special default profile marker
		// this gives external code chance to set this to a profile or to SYSTEM_PROFILE_NONE
		current = SYSTEM_PROFILE_DEFAULT;
	}

	/**
	 * Retrieves
	 */
	container_id size() {
		return 0;
	}


	uint8_t	profileCount();				// return the number of profiles.
	
	/**
	 * Fetches the root container for the currently active profile.
	 */
	Container* rootContainer();
		
	/**
	 * deletes a profile. All profiles with indices larger than this are moved down to one index lower.
	 * All settings for the profile stored in persistent storage are removed and the space is freed up.
	 * If the current profile is the one being deleted, the profile is deactivated first. 
	 */
	void deleteProfile(uint8_t profile) {
		if (profile==currentProfile()) {
			deactivateCurrentProfile();
		}		
	}		
	
	/**
	 * Activate the default (previous) profile.
	 */
	void activateDefaultProfile() {}
	
	/**
	 * Activate the selected profile.
	 */
	bool activateProfile(int8_t index);
	
	/**
	 * Deactivate the current profile by deleting all objects. (TODO: ideally this should be in reverse order, but I'm counting on objects not being
	 active during this time and that they have no resources to clean up.)
	 */
	void deactivateCurrentProfile();
	
	/**
	 * Returns the id of the current profile, or -1 if no profile is active.
	 * @return The currently active profile index, or -1 if no profile is active.
	 */
	uint8_t currentProfile() { return current; }

	bool isCurrentProfileOpen() {
		return currentProfile()>=0 && (currentProfile()==profileCount()-1);
	}

};

