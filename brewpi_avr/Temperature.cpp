
#include "Temperature.h"

temperature constrainTemp(long_temperature valLong, temperature lower, temperature upper){
	temperature val = constrainTemp16(valLong);
	
	if(val < lower){
		return lower;
	}
	
	if(val > upper){
		return upper;
	}
	return temperature(valLong);
}


temperature constrainTemp16(long_temperature val)
{
	if(val<MIN_TEMP){
		return MIN_TEMP;
	}
	if(val>MAX_TEMP){
		return MAX_TEMP;
	}
	return val;
}

temperature multiplyFactorTemperatureLong(temperature factor, long_temperature b)
{
	return constrainTemp16(((long_temperature) factor * (b-C_OFFSET))>>TEMP_FIXED_POINT_BITS);
}

temperature multiplyFactorTemperatureDiffLong(temperature factor, long_temperature b)
{
	return constrainTemp16(((long_temperature) factor * b)>>TEMP_FIXED_POINT_BITS);
}


temperature multiplyFactorTemperature(temperature factor, temperature b)
{
	return constrainTemp16(((long_temperature) factor * ((long_temperature) b - C_OFFSET))>>TEMP_FIXED_POINT_BITS);
}

temperature multiplyFactorTemperatureDiff(temperature factor, temperature b)
{
	return constrainTemp16(((long_temperature) factor * (long_temperature) b )>>TEMP_FIXED_POINT_BITS);
}
