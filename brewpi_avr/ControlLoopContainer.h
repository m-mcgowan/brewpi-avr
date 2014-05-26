#pragma once

#include "Values.h"

#define CONTROL_LOOP_MAX_ITEMS 4
#define CONTROL_LOOP_ITEM_EEPROM_SIZE 4

extern void logValues(Object* target, container_id id);

class ControllerLoopInfo : public EepromValue
{	
	uint32_t	nextLoopTime;
	prepare_t	prepare;
	uint8_t		state;
	uint8_t		logCount;
	
	// persisted data from here
	uint8_t		flags;
	uint16_t	period;

	/**
	 * Read state from eeprom.
	 */
	void load() {
		eepromAccess.readBlock(&flags, address, 3);
		nextLoopTime = ticks.millis();
		state = 0;
		logCount = 0;
	}

public:
	/**
	 * @param initialize	When true, the persistent storage is initialized.
	 * @param address		The address of the persistent storage.
	 */
	ControllerLoopInfo(uint8_t initialize, eptr_t address) {
		if (initialize) {
			writePointer(address, 0);
			writePointer(address+2, 0);
		}
		rehydrated(address);
	}
	
	
	void rehydrated(eptr_t address) {
		EepromValue::rehydrated(address);
		load();
	}

	bool isEnabled() {
		return flags & 0x8;
	}

	uint8_t logPeriod() {
		uint8_t exponent = flags & 0x7;
		return exponent ? 1<<(exponent-1) : 0;
	}

	uint16_t loopPeriod() {
		return period;
	}	

	void writeMaskedFrom(DataIn& data, DataIn& mask) {
		EepromValue::writeMaskedFrom(data, mask);
		load();
	}

	uint8_t streamSize() { return CONTROL_LOOP_ITEM_EEPROM_SIZE; }	
		
	void handleLoop(Object* target, container_id id) {
		if (!isEnabled())
			return;
			
		uint32_t now = ticks.millis();
		uint32_t update = nextLoopTime;
		if (state)
			update += prepare;
		if (update>now)
			return;
		
		state ^= state;
		if (state) {		// prepare not called, so do that first
			prepare = target->prepare();
		} else { // prepare already called
			nextLoopTime += period;
			target->update();
			uint8_t log_period = logPeriod();
			if (log_period) {			// logging enabled
				if (!logCount) {
					logValues(target, id);
					this->logCount = log_period;
				}
				this->logCount--;
			}
		}		
	}	
};

/**
 * The container that provides the associated configuration values for items in the main
 * control loop container. Item at index 0 is reserved (will return NULL.)
 * The item at index N (N>0) is the configuration for the item at index N in the ControlLoopContainer.
 * Physically, the config values are stored at index ((N-1)/2)+1 in the backing container.
 */
class ControlLoopConfigContainer : public Container
{
	Container* container;
		
public:
	ControlLoopConfigContainer(Container* source) : container(source) {}
	
	Object* item(container_id id) {
		// item 0 is reserved. item 1..N+1 are at odd indices in the backing container
		if (id==0)
			return NULL;
		return container->item(((id-1)*2)+1);
	}
	
	container_id size() {
		return (container->size()/2)+1;
	}
		
};

/**
 * Implements a container where each item is treated as a separate control loop.
 * The container uses one dynamic container as backing for storage of the contained objects plus
 * their associated configuration. 
 * The logical view is that item 0 in the main container is the control loop configuration container. 
 * Items 1..N in the main container are the control loop objects, each one executed as a separate control loop. 
 * Corresponding configuration for each control loop N is at item N in the configuration container.
 * 
 * The physical mapping is that slot 2(N-1) stores the control loop object, and slot 2(N-1)+1 stores the
 * control loop config.
 */
class ControlLoopContainer : public OpenContainer
{
	/**
	 * The backing container for the items in this container.
	 * The first item is the ControlLoopConfigContainer which is used
	 * to store the loop config.
	 */
	DynamicContainer			container;
	ControlLoopConfigContainer	config;
	eptr_t						address;
	uint8_t						rehydrated_flag;
	
public:			
	ControlLoopContainer(eptr_t _address) : config(&container), address(_address), rehydrated_flag(0) { }

	void rehydrated(eptr_t address)
	{
		rehydrated_flag = 1;
	}
	
	/**
	 * Adds a new item to this container at the given index. Any existing item at that index is
	 * Removed, along with it's associated configuration.
	 * If this container has not been rehydrated (rehydrated_flag==0) then the ControllerLoopInfo are
	 * not initialized, preserving the values previous persisted. 
	 * Once the root container has been fully instantiated, the rehydrated_flag is set and subsequent
	 * added items are assumed to be new.
	 */
	bool add(container_id index, Object* item) {		
		uint8_t idx = (index-1)*2;
		if (idx<CONTROL_LOOP_MAX_ITEMS*2 && container.add(idx, item)) {						
			// todo - what if this add fails (e.g. out of memory?)
			container.add(idx+1, new_object(ControllerLoopInfo(rehydrated_flag, address+(CONTROL_LOOP_ITEM_EEPROM_SIZE*(index-1)))));
			return true;
		}
		return false;
	}
	
	/**
	 * Removes an item at the given slot.
	 * The item providing the control loop is removed as well as the corresponding configuration.
	 */
	void remove(container_id index) {
		if (index>0) {
			index--;
			container.remove(index*2);
			container.remove((index*2)+1);
		}
	}
		
	/**
	 * Determines the next available free slot in this container.
	 * @return A value greater or equal to zero - the next available free slot. Negative value
	 *	indicates no more free slots.
	 */
	container_id next() { return (container.next()/2)+1; }

	container_id size() {
		return (container.size()/2)+1;
	}
		
	Object* item(container_id id) {
		// item 0 maps to config
		if (id==0)
			return &config;
		// subsequent items map to the contained objects at even offsets
		return container.item((id-1)*2);		
	}
		
	void update() {
		for (int i=0; i<container.size(); i++) {
			Object* item = container.item(i++);
			ControllerLoopInfo* config = (ControllerLoopInfo*)container.item(i);
			if (config)
				config->handleLoop(item, i>>1);
		}
	}
};