#if !defined(AVS_DEBUG_H_)
#define AVS_DEBUG_H_

#include <stdarg.h>

int printf(const char* string, ...);
void avs_debug_blackhole (const char string[]);
void avs_debug_change_putstring (void (*putstring_ptr) (const char8 string[]));
uint8 Debug_UART_ReadTxStatus();

extern char avs_debug_msg[100];
extern void (*avs_debug_putstring) (const char string[]);

#endif