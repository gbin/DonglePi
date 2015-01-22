#include <asf.h>
#include "uart.h"
#include "pins.h"
#include "dbg.h"

/*
   message UART {
    enum Speed {
      BAUD_RATE_115200 = 115200;
    }
    enum Chr {
      _5 = 5;
      _6 = 6;
      _7 = 7;
      _8 = 8;
      _9 = 9;
    }
    enum Parity {
      NONE = 0;
      ODD = 1;
      EVEN = 2;
    }
    enum StopBits {
      _1 = 1;
      _2 = 2;
    }
    required bool enabled = 1;
    required Speed speed = 2 [default = BAUD_RATE_115200];
    required Chr chr = 3 [default = _8];
    required Parity parity = 4 [default = NONE];
    required StopBits stop_bits = 5 [default = _1];
  }
*/

#if !SERIAL_DEBUG
struct usart_module usart_module;
struct usart_config usart_conf;
#endif

static void uart_config(Config_UART config) {
#if !SERIAL_DEBUG
  l("UART enabled");
  usart_get_config_defaults(&usart_conf);
  switch (config.speed) {
    case Config_UART_Speed_BAUD_RATE_115200:
      usart_conf.baudrate = 115200;
    break;
    // TODO: other speeds
  }
  switch (config.chr) {
    case Config_UART_Chr_B5:
      usart_conf.character_size = USART_CHARACTER_SIZE_5BIT;
      break;
    case Config_UART_Chr_B6:
      usart_conf.character_size = USART_CHARACTER_SIZE_6BIT;
      break;
    case Config_UART_Chr_B7:
      usart_conf.character_size = USART_CHARACTER_SIZE_7BIT;
      break;
    case Config_UART_Chr_B8:
      usart_conf.character_size = USART_CHARACTER_SIZE_8BIT;
      break;
    case Config_UART_Chr_B9:
      usart_conf.character_size = USART_CHARACTER_SIZE_9BIT;
      break;
  }
  switch (config.parity) {
    case Config_UART_Parity_NONE:
      usart_conf.parity = USART_PARITY_NONE;
      break;
    case Config_UART_Parity_ODD:
      usart_conf.parity = USART_PARITY_ODD;
      break;
    case Config_UART_Parity_EVEN:
      usart_conf.parity = USART_PARITY_EVEN;
      break;
  }
  switch (config.stop_bits) {
    case Config_UART_StopBits_S1:
      usart_conf.stopbits = USART_STOPBITS_1;
      break;
    case Config_UART_StopBits_S2:
      usart_conf.stopbits = USART_STOPBITS_2;
      break;
  }

  usart_conf.mux_setting = USART_RX_3_TX_2_XCK_3;
  usart_conf.pinmux_pad0 = PINMUX_UNUSED;
  usart_conf.pinmux_pad1 = PINMUX_UNUSED;
  usart_conf.pinmux_pad2 = PINMUX_PA14C_SERCOM2_PAD2;
  usart_conf.pinmux_pad3 = PINMUX_PA15C_SERCOM2_PAD3;
  while (usart_init(&usart_module, SERCOM2, &usart_conf) != STATUS_OK) {
  }
  usart_enable(&usart_module);
#endif
}

bool handle_uart_config_cb(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    l("Configuration for uart...");
    Config_UART uart;
    if (!pb_decode(stream, Config_UART_fields, &uart)) {
      l("Failed to decode a uart configuration");
    }
    if (switch_uart(uart.enabled)) {
      if (uart.enabled) {
        uart_config(uart);
      } else {
#if !SERIAL_DEBUG
        usart_disable(&usart_module);
#endif
        l("usart disabled");
      }
    } else {
      l("UART cannot be enabled/disabled");
    }
    return true;
}

bool handle_uart_write_cb(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  //usart_write_buffer_wait(&usart_module, (const uint8_t *)string, size);
  return true;
}


