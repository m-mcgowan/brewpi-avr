#pragma once

#include "Values.h"
#include "Memops.h"


/**
 * A container with space statically allocated.
 */
template<int SIZE> class StaticContainer : public Container
{
	private:
		Object* _items[SIZE];	// the items in this container.
		
		container_id freeSlot() {
			for (int i=0; i<SIZE;i++)
				if (!_items[i])
					return i;
			return -1;
		}
		
	public:
		StaticContainer() {
			clear((uint8_t*)_items, SIZE*sizeof(Object*));
		}
			
		Object* item(container_id id) {
			return _items[id];
		}
		
		container_id add(Object* item) {
			container_id slot = freeSlot();
			if (slot>=0) {				
				delete _items[slot];
				_items[slot] = item;				
			}
			return slot;
		}		
		
		void remove(container_id id) {			
			delete _items[id];
			_items[id] = NULL;
		}
		
		/*
		 * The number of items in this container.
		 */
		container_id size() { return SIZE; }
		
};