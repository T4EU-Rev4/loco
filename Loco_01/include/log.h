#ifndef LOG_h
#define LOG_h


#include <U8x8lib.h>


// The size (width * height) depends on the display 
#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8


void log_setup();
void log_print(const String &s);
void log_print(const char str[]);
void log_line( uint8_t line, const char *s);

#endif