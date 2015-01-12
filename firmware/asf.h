#ifndef ASF_H
#define ASF_H

// From module: Common SAM0 compiler driver
#include <compiler.h>
#include <status_codes.h>

// From module: EXTINT - External Interrupt (Callback APIs)
#include <extint.h>
#include <extint_callback.h>

// From module: Generic board support
#include <board.h>

// From module: Interrupt management - SAM implementation
#include <interrupt.h>

// From module: PORT - GPIO Pin Control
#include <port.h>

// From module: Part identification macros
#include <parts.h>

#include <usb_dual.h>
#include <usb.h>
#include <sercom.h>
// #include <sercom_interrupt.h>
#include <usart.h>
// #include <usart_interrupt.h>

// From module: SYSTEM - Clock Management for SAMD21
#include <clock.h>
#include <gclk.h>

// From module: SYSTEM - Core System Driver
#include <system.h>

// From module: SYSTEM - I/O Pin Multiplexer
#include <pinmux.h>

// From module: SYSTEM - Interrupt Driver
#include <system_interrupt.h>

#include <samd/sleepmgr.h>
#include <sleepmgr.h>
#include <usb_protocol_cdc.h>
#include <udi_cdc.h>
#include <udc.h>
#include <udd.h>

// i2c stuff
#include <i2c_master.h>
#endif // ASF_H
