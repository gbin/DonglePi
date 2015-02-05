#include <asf.h>
#include "dbg.h"
#include "spi.h"
#include "pins.h"

struct spi_module spi_master_instance;
struct spi_slave_inst slave;

static void w(uint8_t num, ...) {
  static uint8_t sbuffer[5];
  va_list args;
  va_start(args, num);
  for (int x = 0; x < num; x++) sbuffer[x] = (uint8_t)va_arg(args, int);
  va_end(args);

  l("Write (%d) %x %x %x", num, sbuffer[0], sbuffer[1], sbuffer[2]);
  if (spi_write_buffer_wait(&spi_master_instance, sbuffer, num) != STATUS_OK) {
    l("Error writing");
  }
}

static void spi_config(Config_SPI psi_proto_config) {
  // SPI test
  struct spi_config config_spi_master;
  struct spi_slave_inst_config slave_dev_config;
  /* Configure and initialize software device instance of peripheral slave */
  spi_slave_inst_get_config_defaults(&slave_dev_config);
  slave_dev_config.ss_pin = PIN_PA10;
  spi_attach_slave(&slave, &slave_dev_config);
  /* Configure, initialize and enable SERCOM SPI module */
  spi_get_config_defaults(&config_spi_master);
  config_spi_master.mux_setting = SPI_SIGNAL_MUX_SETTING_N;
  /* Configure pad 0 for MOSI */
  config_spi_master.pinmux_pad0 = PINMUX_PA08C_SERCOM0_PAD0;
  /* Configure pad 1 for MISO */
  config_spi_master.pinmux_pad1 = PINMUX_PA09C_SERCOM0_PAD1;
  /* Configure pad 2 for CS so GPIO */
  config_spi_master.pinmux_pad2 = PINMUX_UNUSED;
  /* Configure pad 3 for SCK */
  config_spi_master.pinmux_pad3 = PINMUX_PA11C_SERCOM0_PAD3;

  if (spi_init(&spi_master_instance, SERCOM0, &config_spi_master) !=
      STATUS_OK) {
    l("SPI init failed");
  }
  spi_enable(&spi_master_instance);

  if (spi_select_slave(&spi_master_instance, &slave, true) != STATUS_OK) {
    l("spi_select_slave error");
  }

  // TODO just a test push that correctly in write
  l("reeeeeset");
  port_pin_set_output_level(PIN_PA22, 0);
  cpu_delay_s(5);
  l("done");
  port_pin_set_output_level(PIN_PA22, 1);

  port_pin_set_output_level(PIN_PA00, 0);
  l("DC in command");
  cpu_delay_s(1);
  w(1, 0xAE);         // DISPLAY_OFF
  w(2, 0xD5, 0x80);   // SET_DISPLAY_CLOCK_DIV, 0x80
  w(2, 0xA8, 0x1F);   // SET_MULTIPLEX, 0x1F
  w(2, 0xDA, 0x02);   // SET_COM_PINS, 0x02
  w(2, 0xD3, 0x00);   // SET_DISPLAY_OFFSET, 0x00
  w(1, 0x40);         // SET_START_LINE | 0x00
  w(2, 0x8D, 0x14);   // CHARGE_PUMP, 0x14
  w(2, 0x20, 0x00);   // SET_MEMORY_MODE, 0x00
  w(1, 0xA0 | 0x01);  // SEG_REMAP | 0x01
  w(1, 0xC8);         // COM_SCAN_DEC
  w(2, 0x81, 0x8F);   // SET_CONTRAST, 0x8f
  w(2, 0xD9, 0xF1);   // SET_PRECHARGE, 0xF1
  w(2, 0xDB, 0x40);   // SET_VCOM_DETECT, 0x40
  w(1, 0xA4);         // DISPLAY_ALL_ON_RESUME
  w(1, 0xA6);         // NORMAL_DISPLAY
  w(1, 0xAF);         // DISPLAY_ON
  //  Draw
  w(2, 0x20, 0x01);  // SET_MEMORY_MODE, MEMORY_MODE_VERT
  w(3, 0x22, 0, 0);  // SET_PAGE_ADDRESS, page_start, page_end
  w(3, 0x21, 0, 0);  // SET_COL_ADDRESS, col_start, col_end
  port_pin_set_output_level(PIN_PA00, 1);
  l("DC in data");
  cpu_delay_s(1);
  w(3, 0xFF, 0xFF, 0xFF);  // self.data

  if (spi_select_slave(&spi_master_instance, &slave, false) != STATUS_OK) {
    l("spi_*DE*select_slave error");
  }

  l("SPI done");
}


bool handle_spi_config_cb(pb_istream_t *stream, const pb_field_t *field,
                          void **arg) {
}

static bool handle_spi_write_data_cb(pb_istream_t *stream,
                                     const pb_field_t *field, void **arg) {
  return true;
}


bool handle_spi_write_cb(pb_istream_t *stream, const pb_field_t *field,
                         void **arg) {
  return true;
}
