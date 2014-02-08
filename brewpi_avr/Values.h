#pragma once

#include "Brewpi.h"
#include "DataStream.h"

typedef int8_t container_id;

const container_id INVALID_ID = (container_id)(-1);

typedef uint16_t prepare_t;

enum ObjectType {
	otObject = 0,
	otContainer = 1,
	otValue = 2,
	
	otWritable = 16		// flag for writable values
};

typedef uint8_t object_t;


struct Object
{
	virtual object_t objectType() {
		return otObject;
	}

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

struct Container : public Object
{
	object_t objectType() { return otContainer; }
	
	/*
	 * Add the given object to the container at the next available slot.
	 * Note that slots are essentially arbitrary. The container guarantees
	 * the object will be available at the slot until removed.
	 */
	container_id add(Object* item) { return INVALID_ID; }
	void remove(container_id id) { }
	
	/**
	 * Fetches the object with the given id.
	 * /return the object with that id, which may be null.
	 */
	Object* item(container_id id) { return NULL; }
	
	/*
	 * The maximum number of items in this container. An item may return NULL. This is provided so callers know
	 * how many values to iterate for this container.
	 */
	container_id size() { return 0; }
};

struct StreamReadable {
	
	virtual void readTo(DataOut& out)=0;
};

struct StreamWritable {
	virtual void writeFrom(DataIn& in)=0;
};

template<class T> struct Readable : public virtual StreamReadable
{		
	virtual T read() { return T(0); }
};

template<class T> struct Writable : public virtual StreamWritable
{
    virtual void write(T t) {}
};

struct AbstractValue : public virtual Object
{
	virtual object_t objectType() { return otValue; }
};

struct AbstractStreamValue : public AbstractValue, virtual StreamReadable, virtual StreamWritable {
		
};

template<class T> class BasicReadOnlyValue : 
	public AbstractStreamValue, virtual public Readable<T>
{
    protected:
        T value;
        
    public:
        BasicReadOnlyValue(T initial=0) 
        : value(initial)
        {}
        
        virtual T read() 
        {
            return value;
        }
		
		virtual void readTo(DataOut& out) 
		{
			out.write((uint8_t*)&value, sizeof(value));
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


template<class T> struct BasicValue : public BasicReadOnlyValue<T>, virtual Writable<T>
{	
	BasicValue(T initial=0)
	: BasicReadOnlyValue<T>(initial)
	{}
		
	object_t objectType() {
		return otValue | otWritable;
	}
};



inline bool isContainer(Object* o)
{
	return o!=NULL && o->objectType()==otContainer;
}


inline bool isReadable(Object* o)
{
	return o->objectType()==otValue;
}

inline bool isWritable(Object* o)
{
	uint8_t ot = o->objectType();
	return ot==otValue && (ot&otWritable);
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