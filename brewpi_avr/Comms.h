#pragma once


/*
 * Comms.h
 *
 * Created: 05/02/2014 22:17:46
 *  Author: mat
 */ 

class Comms {

public:
	static void init();		
	
	/*
	 * Read and process from the commms link.
	 */
	static void receive();
	
};

