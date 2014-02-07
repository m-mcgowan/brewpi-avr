#pragma once

/*
 * Commands.h
 *
 * Created: 05/02/2014 22:32:25
 *  Author: mat
 */ 

#include "DataStream.h"
#include "Values.h"

void handleCommand(DataIn& data, DataOut& out);

Container* rootContainer();