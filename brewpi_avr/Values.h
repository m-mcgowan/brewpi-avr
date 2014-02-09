#pragma once

#include "Brewpi.h"
#include "DataStream.h"

typedef int8_t container_id;

const container_id INVALID_ID = (container_id)(-1);

typedef uint16_t prepare_t;

enum ObjectType {		
	otValue = 4,		// 0x000001xx are for value types. Base value type is stream only readable.
	otValueWrite = 5,	// value is writable (either state and/or stream as indicated.)
	otValueState = 6,	// value state is readable 
	otValueWriteState = 7,	// value state is writable (and readable) and streamable
	otWritableFlag = 1,		// flag for writable values
	otValueStateFlag = 2,	// flag for values that can get set state
	otContainer = 8,
	otOpenContainerFlag =1,	// value to flag that a container supports the OpenContainer interface
};

typedef uint8_t object_t;


struct Object
{
	virtual object_t objectType()=0;

	/**
	 * Prepare this object for subsequent updates. 
	 * The returned value is the number of milliseconds the object needs before updates can be performed.
	 */
	virtual prepare_t prepare() { return 0; }
	
	/**
	 * Called after prepare to update this object's state.
	 */
	virtual void update() { }
	
	virtual ~Object() {}
};

const uint8_t MAX_CONTAINER_DEPTH = 8;

/**
 * A container that exposes a indexed set of contained items.
 */
struct Container : public Object
{
	virtual object_t objectType() { return otContainer; }

	/**
	 * Fetches the object with the given id.
	 * /return the object with that id, which may be null.
	 *
	 * After retrieving the item, callers must call returnItem()
	 */
	virtual Object* item(container_id id) { return NULL; }
		
	/**
	 * Returns a previously fetched item back the container.
	 */
	virtual void returnItem(Object* item) { }
	
	/*
	 * The maximum number of items in this container. Calling {@link #item()} at an index less than this value 
	 * may return {@code NULL}. This is provided so callers know
	 * the upper limit for indexes to iterate over for this container.
	 */
	virtual container_id size() { return 0; }
		
};

/**
 * A container of objects that may support being added to. 
 */
struct OpenContainer : public Container
{
	
	/*
	 * Add the given object to the container at the given slot.
	 * The container guarantees the object will be available at the slot until removed.
	 * @param index	The index of the slot. >=0.
	 * @param item	The object to add.
	 * @return non-zero on success, zero on error. 
	 */
	virtual bool add(container_id index, Object* item) { return false; }
		
	/**
	 * Determines the next available free slot in this container.
	 * @return A value greater or equal to zero - the next available free slot. Negative value
	 *	indicates no more free slots.
	 */
	virtual container_id next() { return -1; }
		
	/**
	 * Removes the item at the given index. 
	 * @param id	The id of the item to remove.
	 * If there is no item at the given index, or the item has already been removed the method does nothing.
	 */
	virtual void remove(container_id id) { }
	
};

/**
 * Abstract class implemented by objects that can write their state to a stream.
 */
struct StreamReadable {	
	virtual void readTo(DataOut& out)=0;
	virtual uint8_t streamSize()=0;			// the size this value occupies in the stream.
};

struct StreamWritable {
	virtual void writeFrom(DataIn& in)=0;
};

struct Value : public Object, public StreamReadable {
	
	virtual object_t objectType() { return otValue; }	// basic value type - read only stream
	
};


/**
 * Classes that can provide a representation of their state implement this interface.
 */
template<class T> struct Readable
{		
	virtual T read()=0;
};

/**
 * Classes that can update their internal state from a given value implement this interface.
 */
template<class T> struct Writable
{
    virtual void write(T t)=0;
};

template<class T> class BasicReadValue : 
	public Value, public Readable<T>
{
    protected:
        T value;
        
    public:
        BasicReadValue(T initial=0) 
        : value(initial)
        {}
        
		virtual object_t objectType() {
			return otValue | otValueStateFlag | otWritableFlag;
		}
		
        virtual T read() 
        {
            return value;
        }
		
		virtual void readTo(DataOut& out) 
		{			
			out.write((uint8_t*)&value, sizeof(value));
		}
	
		virtual uint8_t streamSize() { return sizeof(this->value); }
};


template<class T> struct BasicReadWriteValue : BasicReadValue<T>, Writable<T>, StreamWritable
{	
	BasicReadWriteValue(T initial=0)
	: BasicReadValue<T>(initial)
	{}
		
	virtual object_t objectType() {
		return otValue | otValueStateFlag | otWritableFlag;
	}
	
	virtual void write(T t)
	{
		this->value = t;
	}
	
	virtual void writeFrom(DataIn& in)
	{
		in.read((uint8_t*)&this->value, sizeof(this->value));
	}
		
};

inline bool isContainer(Object* o)
{
	return o!=NULL && (o->objectType() & otContainer);
}

inline bool isOpenContainer(Object* o)
{
	return o!=NULL && (o->objectType() & (otContainer|otOpenContainerFlag));
}

inline bool isReadable(Object* o)
{
	return o!=NULL && (o->objectType() & otValue);
}

inline bool isWritable(Object* o)
{
	return o!=NULL && (o->objectType() & otWritableFlag);	
}


/*
 * Callback function for enumerating objects.
 * The function can return true to stop enumeration. 
 */
typedef bool (*EnumObjectsFn)(Object* obj, void* data, container_id* id);


bool walkContainer(Container* c, EnumObjectsFn callback, void* data, container_id* id, container_id* end);

bool walkObject(Object* obj, EnumObjectsFn callback, void* data, container_id* id, container_id* end);

inline bool walkRoot(Container* c, EnumObjectsFn callback, void* data, container_id* id) {
	return walkContainer(c, callback, data, id, id);
}

/**
 * The host app should provide the root container.
 */
Container* rootContainer();