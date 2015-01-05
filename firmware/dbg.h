#ifndef _DBG_H_
#define _DBG_H_
#include <stdarg.h>
#include <stdio.h>

void log_init(void); 

void l(const char * format, ... ); 
#endif
