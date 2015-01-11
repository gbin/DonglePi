#include <asf.h>
#include "dbg.h"

#define DEBUG_LED1 PIN_PA28
#define DEBUG_BUTTON PIN_PA27

struct usart_module usart_module;
struct usart_config usart_conf;

void l(const char * format, ... ) {
  char string[512];
  va_list argptr;
  va_start(argptr, format);
  int size = vsprintf(string, format, argptr);
  va_end(argptr);
  usart_write_buffer_wait(&usart_module, (const uint8_t *)string, size);
  usart_write_buffer_wait(&usart_module, (const uint8_t *)"\r\n", 2);
}

void log_init() {
  usart_get_config_defaults(&usart_conf);
  usart_conf.stopbits = USART_STOPBITS_1;
  usart_conf.parity = USART_PARITY_NONE;
  usart_conf.character_size = USART_CHARACTER_SIZE_8BIT;

  usart_conf.baudrate = 115200;
  usart_conf.mux_setting = USART_RX_3_TX_2_XCK_3;
  usart_conf.pinmux_pad0 = PINMUX_UNUSED;
  usart_conf.pinmux_pad1 = PINMUX_UNUSED;
  usart_conf.pinmux_pad2 = PINMUX_PA14C_SERCOM2_PAD2;
  usart_conf.pinmux_pad3 = PINMUX_PA15C_SERCOM2_PAD3;
  while (usart_init(&usart_module, SERCOM2, &usart_conf) != STATUS_OK) {
  }
  usart_enable(&usart_module);
  l("-----------  DonglePi debug console ------------");
}

void on1() {
  port_pin_set_output_level(DEBUG_LED1, 1);
}

void off1() {
  port_pin_set_output_level(DEBUG_LED1, 0);
}

bool get_button() {
  return port_pin_get_input_level(DEBUG_BUTTON);
}

