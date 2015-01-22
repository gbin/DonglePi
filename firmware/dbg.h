#ifndef _DBG_H_
#define _DBG_H_
#include <stdarg.h>
#include <stdio.h>

#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG true
#endif
void log_init(void);
void l(const char * format, ... );
void on1(void);
void off1(void);
void on2(void);
void off2(void);
bool get_button(void);
#endif
