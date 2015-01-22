#include "uart.h"

// gbin: kept here to implement the usart <-> USB bridge
/*
struct usart_module usart_module_edbg;
struct usart_config usart_conf;

static uint8_t tx_data;
static uint8_t rx_data;

static void usart_tx_callback(const struct usart_module *const module)
{
  if (udi_cdc_is_rx_ready()) {
    ui_com_rx_start();
    tx_data = udi_cdc_getc();
    usart_write_buffer_job(&usart_module_edbg, &tx_data, 1);
  } else {
    usart_disable_callback(&usart_module_edbg, USART_CALLBACK_BUFFER_TRANSMITTED);
    ui_com_rx_stop();
  }
}


static void usart_rx_callback(const struct usart_module *const module)
{
  ui_com_tx_start();

  if (!udi_cdc_is_tx_ready()) {
    udi_cdc_signal_overrun();
    ui_com_overflow();
  } else {
    udi_cdc_putc(rx_data);
  }

  ui_com_tx_stop();

  usart_read_buffer_job(&usart_module_edbg, &rx_data, 1);

  return;
}



void uart_config(uint8_t port,usb_cdc_line_coding_t *cfg)
{
  UNUSED(port);
  usart_get_config_defaults(&usart_conf);

  switch (cfg->bCharFormat) {
    case CDC_STOP_BITS_2:
      usart_conf.stopbits = USART_STOPBITS_2;
      break;

    case CDC_STOP_BITS_1_5:
      usart_conf.stopbits = USART_STOPBITS_1;
      break;

    case CDC_STOP_BITS_1:
    default:
      usart_conf.stopbits = USART_STOPBITS_1;
      break;
  }

  switch (cfg->bParityType) {
    case CDC_PAR_EVEN:
      usart_conf.parity = USART_PARITY_EVEN;
      break;

    case CDC_PAR_ODD:
      usart_conf.parity = USART_PARITY_ODD;
      break;

    case CDC_PAR_MARK:
      usart_conf.parity = USART_PARITY_NONE;
      break;

    case CDC_PAR_SPACE:
      usart_conf.parity = USART_PARITY_NONE;
      break;

    case CDC_PAR_NONE:
    default:
      usart_conf.parity = USART_PARITY_NONE;
      break;
  }

  switch(cfg->bDataBits) {
    case 5:
      usart_conf.character_size = USART_CHARACTER_SIZE_5BIT;
      break;
    case 6:
      usart_conf.character_size = USART_CHARACTER_SIZE_6BIT;
      break;
    case 7:
      usart_conf.character_size = USART_CHARACTER_SIZE_7BIT;
      break;
    case 8:
    default:
      usart_conf.character_size = USART_CHARACTER_SIZE_8BIT;
      break;
  }

  usart_conf.baudrate = LE32_TO_CPU(cfg->dwDTERate);
  usart_conf.mux_setting = CONF_USART_MUX_SETTING;
  usart_conf.pinmux_pad0 = CONF_USART_PINMUX_PAD0;
  usart_conf.pinmux_pad1 = CONF_USART_PINMUX_PAD1;
  usart_conf.pinmux_pad2 = CONF_USART_PINMUX_PAD2;
  usart_conf.pinmux_pad3 = CONF_USART_PINMUX_PAD3;
  usart_disable(&usart_module_edbg);
  usart_init(&usart_module_edbg, CONF_USART_BASE, &usart_conf);
  usart_enable(&usart_module_edbg);

  usart_register_callback(&usart_module_edbg, usart_tx_callback,
      USART_CALLBACK_BUFFER_TRANSMITTED);
  usart_enable_callback(&usart_module_edbg, USART_CALLBACK_BUFFER_TRANSMITTED);
  usart_register_callback(&usart_module_edbg, usart_rx_callback,
      USART_CALLBACK_BUFFER_RECEIVED);
  usart_enable_callback(&usart_module_edbg, USART_CALLBACK_BUFFER_RECEIVED);
  usart_read_buffer_job(&usart_module_edbg, &rx_data, 1); 
}

void uart_open(uint8_t port)
{
  UNUSED(port);

  usart_disable(&usart_module_edbg);
  usart_init(&usart_module_edbg, CONF_USART_BASE, &usart_conf);
  usart_enable(&usart_module_edbg);

  usart_register_callback(&usart_module_edbg, usart_tx_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
  usart_register_callback(&usart_module_edbg, usart_rx_callback, USART_CALLBACK_BUFFER_RECEIVED);
}

void uart_close(uint8_t port)
{
  UNUSED(port);
  usart_disable(&usart_module_edbg);
}*/
