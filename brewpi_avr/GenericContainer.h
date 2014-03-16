#pragma once

#include "Brewpi.h"             // for max())
#include "Values.h"
#include "Memops.h"


/**
 * A container with space statically allocated.
 */
template<int SIZE> class StaticTemplateContainer : public OpenContainer
{
	private:
		Object* _items[SIZE];	// the items in this container.
		
		container_id freeSlot() {
			for (int i=0; i<SIZE;i++)
				if (!_items[i])
					return i;
			return -1;
		}
		
		void prepare(Object* item, prepare_t& time) {
			if (item)
			time = max(time, item->prepare());
		}

	public:
		prepare_t prepare() {
			prepare_t time = 0;
			for (int i=0; i<size(); i++ ) {
				prepare(item(i), time);
			}
			return time;
		}
	
		virtual void update() {
			for (int i=0; i<size(); i++ ) {
				Object* o = item(i);
				if (o)
				o->update();
			}
		}
		
		StaticTemplateContainer() {
			clear((uint8_t*)_items, SIZE*sizeof(Object*));
		}
			
		Object* item(container_id id) {
			return _items[id];
		}
		
		container_id next() {
			return freeSlot();
		}
		
		bool add(container_id slot, Object* item) {
			if (slot>=SIZE)
				return false;				
			remove(slot);			
			_items[slot] = item;
			return true;
		}
		
		void remove(container_id id) {			
			delete_object(_items[id]);
			_items[id] = NULL;
		}
		
		/*
		 * The number of items in this container.
		 */
		container_id size() { return SIZE; }

#if OBJECT_VIRTUAL_DESTRUCTOR		
		// the contract says that before a container is deleted, the caller should ensure all contained objects
		// are also deleted. (if they were added.)
		~StaticContainer() {
			for (int i=0; i<SIZE;i++)
				delete_object(_items[i]);			
		}
#endif		
		
};




/**
 * A container with space statically allocated.
 */
class FixedContainer : public OpenContainer
{
	private:
		container_id SIZE;
		Object** _items;
		
		container_id freeSlot() {
			for (int i=0; i<size();i++)
				if (!_items[i])
					return i;
			return -1;
		}
		
		void prepare(Object* item, prepare_t& time) {
			if (item)
				time = max(time, item->prepare());
		}

	public:
		prepare_t prepare() {
			prepare_t time = 0;
			for (int i=0; i<size(); i++ ) {
				prepare(item(i), time);
			}
			return time;
		}
	
		virtual void update() {
			for (int i=0; i<size(); i++ ) {
				Object* o = item(i);
				if (o)
				o->update();
			}
		}
		
			
		Object* item(container_id id) {
			return _items[id];
		}
		
		container_id next() {
			return freeSlot();
		}
		
		bool add(container_id slot, Object* item) {
			if (slot>=SIZE)
				return false;				
			remove(slot);			
			_items[slot] = item;
			return true;
		}
		
		void remove(container_id id) {			
			delete_object(_items[id]);
			_items[id] = NULL;
		}
		
		/*
		 * The number of items in this container.
		 */
		container_id size() { return SIZE; }


		FixedContainer(container_id size, Object** items)
		: SIZE(size), _items(items) {
		
			clear((uint8_t*)_items, SIZE*sizeof(Object*));
		}
	

#if OBJECT_VIRTUAL_DESTRUCTOR		
		// the contract says that before a container is deleted, the caller should ensure all contained objects
		// are also deleted. (if they were added.)
		~StaticContainer() {
			for (int i=0; i<SIZE;i++)
				delete_object(_items[i]);			
		}
#endif		
		
};
