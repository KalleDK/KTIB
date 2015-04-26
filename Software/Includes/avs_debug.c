#include <project.h>
#include <stdio.h>
#include <stdarg.h>

char8 avs_debug_msg[100];

void (*avs_debug_putstring) (const char8 string[]) = avs_debug_blackhole;

void avs_debug_blackhole (const char string[])
{
	
}

void avs_debug_change_putstring (void (*putstring_ptr) (const char8 string[]))
{
	avs_debug_putstring = putstring_ptr;
}

int printf(const char* string, ...)
{
    int result;
	 
    va_list args;
    
    va_start(args, string);
    result = vsprintf(avs_debug_msg, string, args);
    va_end(args);
    avs_debug_putstring(avs_debug_msg);
    return result;

}