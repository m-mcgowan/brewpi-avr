#pragma once

typedef uint8_t container_id;
typedef uint16_t prepare_t;

enum ObjectType {
	otObject,
	otContainer,
	otValue
};

class Object
{
	
	virtual ObjectType objectType() {
		return otObject;
	}
	
	virtual prepare_t prepare() { return 0; }
};

class Container : public Object
{
	ObjectType objectType() { return otContainer; }
	
	container_id add(void* item) { }
	void* remove(container_id id) { }
	
	void* item(container_id id) { }
	
	/*
	 * The number of items in this container.
	 */
	container_id size();
	
	
};


template<class T> class Readable
{	
    public:	
	virtual T read() { return T(0); }
	
	virtual ~Readable() {}
};

template<class T> class Writable
{
    public:
        virtual void write(T t) {}
                
};


template<class T> class BasicValue : 
	public Readable, public Writable
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
};




