
#include "Brewpi.h"
#include "Values.h"


bool walkContainer(Object* obj, EnumObjectsFn callback, void* data, container_id* id, container_id* end) 
{
	Container* c = (Container*)obj;
	uint8_t count = c->size();
	for (int8_t i=0; i<count; i++) {
		Object* o = c->item(i);
		*end = i;
		if (walkContainerObjects(c, callback, data, id, end))
			return true;
	}	
	return false;
}

/**
 * Recursively walks all objects in a container hierarchy.
 */
bool walkContainerObjects(Container* obj, EnumObjectsFn callback, void* data, container_id* id, container_id* end) {	
	if (callback(obj, data, id))
		return true;
	
	if (isContainer(obj)) {		
		*end++ |= 0x80;		// flag as not last element
		walkContainer(obj, callback, data, id, end);
		*--end &= 0x7F;		// remove last bit
	}
	return false;
}



