#include <asf.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "protocol/donglepi.pb.h"
#include "conf_usb.h"
#include "board.h"
#include "ui.h"
#include "uart.h"
#include "dbg.h"
#include "i2c.h"
#include "gpio.h"
#include "pins.h"

// This main module takes care of initialization, protocol handling and dispatching.

static volatile bool main_b_cdc_enable = false;

static void setup_led(void) {
  // Configure main LED as output
  struct port_config config_port_pin;
  port_get_config_defaults(&config_port_pin);
  config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
  port_pin_set_config(PIN_PA28, &config_port_pin);
}


int main(void)
{
  system_init();
  log_init();
  l("init vectors");
  irq_initialize_vectors();
  l("irq enable");
  cpu_irq_enable();
  l("sleep mgr start");
  sleepmgr_init();
  l("configure_pins");
  setup_led();
  l("ui_init");
  ui_init();

  l("ui_powerdown");
  ui_powerdown();

  // Start USB stack to authorize VBus monitoring
  l("udc_start");
  udc_start();

  while (true) {
    sleepmgr_enter_sleep();
  }
}

void main_suspend_action(void) {
  l("main_suspend_action");
  off1();
  ui_powerdown();
}

void main_resume_action(void) {
  l("main_resume_action");
  on1();
  ui_wakeup();
}

void main_sof_action(void)
{
  if (!main_b_cdc_enable)
    return;
  // l("Frame number %d", udd_get_frame_number());
}

struct spi_module spi_master_instance;
struct spi_slave_inst slave;

uint8_t sbuffer[] = {0, 0, 0};

static void w(uint8_t num, ...) {
  va_list args;
  va_start (args, num);
  for ( int x = 0; x < num; x++ )     
    sbuffer[x] = (uint8_t) va_arg (args, int);
  va_end ( args );                
  
 l("Write (%d) %x %x %x", num, sbuffer[0], sbuffer[1], sbuffer[2]);
  if(spi_write_buffer_wait(&spi_master_instance, sbuffer, num) != STATUS_OK) {
     l("Error writing");
  }
}

bool main_cdc_enable(uint8_t port)
{
  l("main_cdc_enable %d", port);
  main_b_cdc_enable = true;
  l("config SPI", port);
  
  // DC mapping
  struct port_config config_port_pin;
  port_get_config_defaults(&config_port_pin);
  config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
  port_pin_set_config(PIN_PA00, &config_port_pin); // GPIO17 D/C
  port_pin_set_config(PIN_PA22, &config_port_pin); // GPIO04 RESET
  
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

  if (spi_init(&spi_master_instance, SERCOM0, &config_spi_master)!=STATUS_OK) {
    l("SPI init failed");  
  }
  spi_enable(&spi_master_instance); 

  if(spi_select_slave(&spi_master_instance, &slave, true)!=STATUS_OK) {
    l("spi_select_slave error");
  }
  l("reeeeeset");
  port_pin_set_output_level(PIN_PA22, 0);
  cpu_delay_s(5);
  l("done");
  port_pin_set_output_level(PIN_PA22, 1);

  port_pin_set_output_level(PIN_PA00, 0);
  l("DC in command");
  cpu_delay_s(1);
  w(1, 0xAE);        // DISPLAY_OFF
  w(2, 0xD5, 0x80);  // SET_DISPLAY_CLOCK_DIV, 0x80
  w(2, 0xA8, 0x1F);  // SET_MULTIPLEX, 0x1F
  w(2, 0xDA, 0x02);  // SET_COM_PINS, 0x02
  w(2, 0xD3, 0x00);  // SET_DISPLAY_OFFSET, 0x00
  w(1, 0x40);        // SET_START_LINE | 0x00
  w(2, 0x8D, 0x14);  // CHARGE_PUMP, 0x14
  w(2, 0x20, 0x00);  // SET_MEMORY_MODE, 0x00
  w(1, 0xA0 | 0x01); // SEG_REMAP | 0x01
  w(1, 0xC8);        // COM_SCAN_DEC
  w(2, 0x81, 0x8F);  // SET_CONTRAST, 0x8f
  w(2, 0xD9, 0xF1);  // SET_PRECHARGE, 0xF1
  w(2, 0xDB, 0x40);  // SET_VCOM_DETECT, 0x40
  w(1, 0xA4);        // DISPLAY_ALL_ON_RESUME
  w(1, 0xA6);        // NORMAL_DISPLAY
  w(1, 0xAF);        // DISPLAY_ON
  //  Draw 
  w(2, 0x20, 0x01);  // SET_MEMORY_MODE, MEMORY_MODE_VERT
  w(3, 0x22, 0, 0);  // SET_PAGE_ADDRESS, page_start, page_end
  w(3, 0x21, 0, 0);  // SET_COL_ADDRESS, col_start, col_end
  port_pin_set_output_level(PIN_PA00, 1);
  l("DC in data");
  cpu_delay_s(1);
  w(3, 0xFF, 0xFF, 0xFF); // self.data 

  
  if(spi_select_slave(&spi_master_instance, &slave, false) != STATUS_OK) {
    l("spi_*DE*select_slave error");
  }

  l("SPI done");
  return true;
}

void main_cdc_disable(uint8_t port)
{
  l("main_cdc_disable %d", port);
  main_b_cdc_enable = false;
}

void main_cdc_set_dtr(uint8_t port, bool b_enable) {
}

void ui_powerdown(void) {
}

void ui_init(void) {
}

void ui_wakeup(void) {
}

void cdc_config(uint8_t port, usb_cdc_line_coding_t * cfg) {
  l("cdc_config [%d]", port);
}

#define USB_BUFFER_SIZE 1024
static uint8_t buffer[USB_BUFFER_SIZE];

void cdc_rx_notify(uint8_t port) {
  l("cdc_rx_notify [%d]", port);

  uint8_t b = udi_cdc_getc();
  if (b != 0x08) {
    l("Protocol desync");
  }
  l("First byte ok");
  uint32_t offset=0;
  do {
    buffer[offset++] = b;
    b = udi_cdc_getc();
    l("-> 0x%02x", b);
  } while(b & 0x80);
  buffer[offset++] = b;
  // Now we have enough to know the size
  l("Length read, decoding...");
  l("... 0x%02x 0x%02x", buffer[0], buffer[1]);

  pb_istream_t istream = pb_istream_from_buffer(buffer+1, USB_BUFFER_SIZE);
  l("istream bytes_left before %d", istream.bytes_left);
  uint64_t len = 0;
  pb_decode_varint(&istream, &len);
  l("message_length %d", (uint32_t) len);
  l("offset %d", offset);
  udi_cdc_read_buf(buffer + offset, len);
  l("decode message");
  istream = pb_istream_from_buffer(buffer + offset, len);
  DonglePiRequest request = {0};
  request.config.i2c.funcs.decode = handle_i2c_config_cb;
  request.config.uart.funcs.decode = handle_uart_config_cb;
  request.config.gpio.pins.funcs.decode = handle_gpio_pin_config_cb;
  request.data.i2c.writes.funcs.decode = handle_i2c_write_cb;

  if (!pb_decode(&istream, DonglePiRequest_fields, &request)) {
    l("failed to decode the packet, wait for more data");
    return;
  }

  l("Request #%d received", request.message_nb);

  if(request.has_data && request.data.has_gpio) {
    handle_gpio_write(request.data.gpio);
  }

  pb_ostream_t ostream = pb_ostream_from_buffer(buffer, USB_BUFFER_SIZE);
  DonglePiResponse response = {};
  response.message_nb = request.message_nb;
  l("Create response for #%d", response.message_nb);

  handle_gpio_read(&response);

  pb_encode_delimited(&ostream, DonglePiResponse_fields, &response);
  l("Write response nb_bytes = %d", ostream.bytes_written);
  uint32_t wrote = udi_cdc_write_buf(buffer, ostream.bytes_written);
  l("Done. wrote %d bytes", wrote);
}

/* compression test
// const char *source = "This is my input !";
//char dest[200];
//char restored[17];
//LZ4_compress (source, dest, 17);
//LZ4_decompress_fast(dest, restored, 17);
*/

