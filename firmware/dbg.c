#include <asf.h>
#include "dbg.h"

struct usart_module usart_module;
struct usart_config usart_conf;

void l(const char * format, ... ) {
  char string[512]; 
  va_list argptr;
  va_start(argptr, format);
  int size = sprintf(string, format, argptr);
  va_end(argptr);
  usart_write_buffer_wait(&usart_module, string, size);
  usart_write_buffer_wait(&usart_module, "\r\n", 2);
}

void log_init(void) {
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


