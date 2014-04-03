#pragma once

#include "DataStream.h"

void readMaskedBytes(void* data, uint8_t size, DataIn& in, DataIn& mask);

void writeBytes(void* data, uint8_t size, DataOut& out);