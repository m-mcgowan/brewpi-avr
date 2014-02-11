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



#include <stdio.h>
#include <iostream>

#include "Print.h"

#include "windows.h"
#include "conio.h"


Print::Print() {
}

Print::Print(const Print& orig) {
}

Print::~Print() {
}


static int is_pipe = 0;
static HANDLE input_handle = 0;


StdIO::StdIO()
: in(std::cin), out(stdout)
{
    DWORD dw;
  input_handle = GetStdHandle(STD_INPUT_HANDLE);
  is_pipe = !GetConsoleMode(input_handle, &dw);

}
    


void StdIO::print(char c) {
    fputc(c,out);
}
void StdIO::print(const char* c) {
    fputs(c,out);
}
void StdIO::printNewLine() {
    fputs("\r\n", out);
    flush();
}
void StdIO::println() {
    printNewLine();
}


int input_available()
{
  DWORD nchars = 0;
  /* When using Standard C input functions, also check if there
   is anything in the buffer. After a call to such functions,
   the input waiting in the pipe will be copied to the buffer,
   and the call to PeekNamedPipe can indicate no input available.
   Setting stdin to unbuffered was not enough, IIRC */
  if (stdin->_cnt > 0)
    return 1;
  if (is_pipe)
  {
    /* When running under a GUI, you will end here. */
    if (!PeekNamedPipe(input_handle, NULL, 0, NULL, &nchars, NULL))
      /* Something went wrong. Probably the parent program exited.
         Could call exit() here. Returning 1 will make the next call
         to the input function return EOF, where this should be
         caught then. */
      return 1;

    return (nchars != 0);
  }
  else
    return _kbhit() != 0; /* In "text-mode" without GUI */
}

int StdIO::available() { 
    return input_available();
}

int StdIO::peek() {
    if (!input_available())
        return -1;
    
    return in.peek();
}

void StdIO::begin(unsigned long) 
{
}

size_t StdIO::write(uint8_t w) { 
    fputc(w, out);
    return 1;
}

int StdIO::read() {                     
    char c;
    in.get(c);
    return c;
}

void StdIO::flush() { fflush(out); }


