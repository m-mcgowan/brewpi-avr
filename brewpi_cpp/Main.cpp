/*
 * Copyright 2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdio.h"
#include "Brewpi.h"
#include "EepromAccess.h"
#include "unistd.h"

EepromAccess eepromAccess;

// setup and loop are in brewpi_config so they can be reused across projects
extern void setup(void);
extern void loop (void);

static bool quit = false;

void handleReset() {
    quit = true;
}

void saveEeprom(const char* file)
{
    if (file) {
        FILE* f = fopen(file, "wb");
        if (f)
        {
                fwrite(eepromAccess.eepromData(), 1, eepromAccess.eepromLength(), f);
                fclose(f);
        }
    }
}

void saveEepromIfNeeded(const char* file)
{
    if (eepromAccess.changed()) {
        saveEeprom(file);
    }
}

void loadEeprom(const char* source)
{
    int sz = eepromAccess.eepromLength();
    uint8_t* buf = new uint8_t[sz];    
    FILE* f = fopen(source, "rb");
    if (f)
    {
        fread(buf, 1, sz, f);
        fclose(f);
        eepromAccess.writeBlock(0, buf, sz);
    }
    delete[] buf;
}

int main(int argc, const char* argv[])
{
    char buf[256];
    sprintf(buf, "%s.eeprom", argv[0]);    
    loadEeprom(buf);
    
        setup();
	saveEepromIfNeeded(buf);
	for (;!quit;) {
		loop();
                saveEepromIfNeeded(buf);
	}
        saveEepromIfNeeded(buf);
        return 0;
}

