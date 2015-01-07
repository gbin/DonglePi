#include <asf.h>
#include "dbg.h"

#define DEBUG_LED1 PIN_PA28
#define DEBUG_LED2 PIN_PA02
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
  usart_conf.mux_setting = USART_RX_1_TX_0_XCK_1;
  usart_conf.pinmux_pad0 = PINMUX_PA16D_SERCOM3_PAD0;
  usart_conf.pinmux_pad1 = PINMUX_PA17D_SERCOM3_PAD1;
  usart_conf.pinmux_pad2 = PINMUX_UNUSED;
  usart_conf.pinmux_pad3 = PINMUX_UNUSED;
  while (usart_init(&usart_module, SERCOM3, &usart_conf) != STATUS_OK) {
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

void on2() {
  port_pin_set_output_level(DEBUG_LED2, 1);
}

void off2() {
  port_pin_set_output_level(DEBUG_LED2, 0);
}

bool get_button() {
  return port_pin_get_input_level(DEBUG_BUTTON);
}

