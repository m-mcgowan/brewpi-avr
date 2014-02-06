#pragma once

#include "DataStream.h"

typedef uint8_t container_id;
const container_id INVALID_ID = container_id(-1);

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
	Object* remove(container_id id) { return NULL; }
	
	Object* item(container_id id) { return NULL; }
	
	/*
	 * The number of items in this container.
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

struct AbstractValue : public Object
{
	virtual object_t objectType() { return otValue; }
};

class AbstractStreamValue : public AbstractValue, public StreamReadable, public StreamWritable {
			
};

template<class T> class BasicValue : 
	public AbstractStreamValue, Readable<T>, Writable<T>
{
    private:
        T value;
        
    public:
        BasicValue(T initial) 
        : value(initial)
        {}
        
        virtual void write(T t)
        {
            this->value = t;
        }                
        
        virtual T read() 
        {
            return value;
        }
		
		virtual void writeFrom(DataIn& in)
		{
			in.read((uint8_t*)&value, sizeof(value));
		}
		
		virtual void readTo(DataOut& out) 
		{
			out.write((uint8_t*)&value, sizeof(value));
		}
		
};
