
#include "Temperature.h"

char * tempToString(char * s, long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength);
temperature stringToTemp(const char * string);

char * tempDiffToString(char * s, long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength);
temperature stringToTempDiff(const char * string);

char * fixedPointToString(char * s, long_temperature rawValue, uint8_t numDecimals, uint8_t maxLength);
char * fixedPointToString(char * s, temperature rawValue, uint8_t numDecimals, uint8_t maxLength);
long_temperature stringToFixedPoint(const char * numberString);

int fixedToTenths(long_temperature temperature);
temperature tenthsToFixed(int temperature);

// mdma: a quick hack to remove the dependency on TempControl - the temp format 
// code was referring to TempControl::cc.temperatureFormat
// A long-term fix is to add tempFormat as a parameter to all functions that need it.

extern char tempFormat;

#define OPTIMIZE_TEMPERATURE_FORMATS 1 && OPTIMIZE_GLOBAL
