
#include "Brewpi.h"
#include "Values.h"

/**
 * /param obj - Assumed to be a container.
 */
bool walkContainer(Container* c, EnumObjectsFn callback, void* data, container_id* id, container_id* end) 
{
	uint8_t count = c->size();
	for (int8_t i=0; i<count; i++) {
		Object* o = c->item(i);
		*end = i;
		if (walkObject(o, callback, data, id, end))
			return true;
	}	
	return false;
}

/**
 * Recursively walks all objects in a container hierarchy.
 */
bool walkObject(Object* obj, EnumObjectsFn callback, void* data, container_id* id, container_id* end) {	
	if (callback(obj, data, id))
		return true;
	
	if (isContainer(obj)) {		
		*end++ |= 0x80;		// flag as not last element in id chain
		walkContainer((Container*)obj, callback, data, id, end);
		*--end &= 0x7F;		// remove last bit
	}
	return false;
}



