/* 
 * File:   Print.h
 * Author: mat
 *
 * Created on 19 August 2013, 20:40
 */

#ifndef PRINT_H
#define	PRINT_H

#include "stddef.h"
#include "string.h"
#include "stdint.h"


class Print {
public:
    Print();
    Print(const Print& orig);
    virtual ~Print();
    
    // convenience method to print a string
    size_t write(const char *str) {
      if (str == NULL) return 0;
        return write((const uint8_t *)str, strlen(str));
    }
    virtual size_t write(const uint8_t *s, size_t size) {
        size_t done = 0;
        while (size--) {
                done += write(*s++);
        }
        return done;
    }
    
    virtual size_t write(uint8_t c)=0;
    
     size_t print(const char* str)
     {
         return write(str);
     }
    
     size_t print(char c) {
         return write((const uint8_t*)&c, 1);
     }
    
private:

};


class Stream : public Print
{    
};


class StdIO : public Stream {
public:
    StdIO();

    void print(char c);
    void print(const char* c);
    void printNewLine();
    void println();
    int available();
    void begin(unsigned long);

    int read();
    int peek();

    size_t write(uint8_t w);
    size_t write(const uint8_t* data, uint8_t len) {
        write(data, size_t(len));
    }
    void flush();
    operator bool() { return true; }

private:
    std::istream& in;
    FILE* out;
};


#endif	/* PRINT_H */

