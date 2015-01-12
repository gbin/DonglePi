#include <asf.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "protocol/donglepi.pb.h"
#include "conf_usb.h"
#include "board.h"
#include "ui.h"
#include "uart.h"
#include "dbg.h"

static volatile bool main_b_cdc_enable = false;

// RPI GPIO # -> SAMD pin#
static uint8_t pin_map[28] = {
  0,
  0,
  PIN_PA16,  // GPIO02
  PIN_PA17,  // GPIO03
  PIN_PA22,  // GPIO04
  0,
  0,
  PIN_PA02,  // GPIO07
  PIN_PA11,  // GPIO08
  PIN_PA09,  // GPIO09
  PIN_PA08,  // GPIO10
  PIN_PA10,  // GPIO11
  0,
  0,
  PIN_PA14,  // GPIO14
  PIN_PA15,  // GPIO15
  0,
  PIN_PA00,  // GPIO17
  PIN_PA04,  // GPIO18
  0,
  0,
  0,
  PIN_PA01,  // GPIO22
  PIN_PA05,  // GPIO23
  PIN_PA07,  // GPIO24
  PIN_PA23,  // GPIO25
  0,
  PIN_PA06   // GPIO27
};

/* static void configure_systick_handler(void) {
   SysTick->CTRL = 0;
   SysTick->LOAD = 999;
   SysTick->VAL  = 0;
   SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
   }*/

struct i2c_master_module i2c_master;


static void configure_pins(void) {
  // GPIO stuff
  struct port_config config_port_pin;
  port_get_config_defaults(&config_port_pin);
  config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
  port_pin_set_config(PIN_PA28, &config_port_pin);

  // I2C stuff
  l("Init i2c");
  struct i2c_master_config config_i2c_master;
  i2c_master_get_config_defaults(&config_i2c_master);
  config_i2c_master.buffer_timeout = 10000; 
  config_i2c_master.pinmux_pad0 = PINMUX_PA16C_SERCOM1_PAD0;
  config_i2c_master.pinmux_pad1 = PINMUX_PA17C_SERCOM1_PAD1;
  config_i2c_master.baud_rate = I2C_MASTER_BAUD_RATE_100KHZ;

  if (i2c_master_init(&i2c_master, SERCOM1, &config_i2c_master)!=STATUS_OK)
    l("Init Error");
  i2c_master_enable(&i2c_master);

  uint8_t i2cbyte[1] = {0};
  uint8_t i2cbibyte[2] = {0, 0};
  struct i2c_master_packet monobyte = {
    .address = 0x70,
    .data_length = 1,
    .data = i2cbyte,
  };

  struct i2c_master_packet bibyte = {
    .address = 0x70,
    .data_length = 2,
    .data = i2cbibyte,
  };
 
  l("Init display");
  
  i2cbyte[0] = 0x21; // oscillator on
  if (i2c_master_write_packet_wait(&i2c_master, &monobyte) != STATUS_OK)
    l("1 not OK");  
  i2cbyte[0] = 0x81; // on, no blink
  if (i2c_master_write_packet_wait(&i2c_master, &monobyte) != STATUS_OK)
    l("2 not OK");  
  i2cbyte[0] = 0xEF; // brighness
  if (i2c_master_write_packet_wait(&i2c_master, &monobyte) != STATUS_OK)
    l("3 not OK");  

  l("Draw stuff");
  // draw stuff
  for (int i=0; i<8; i++) {
    i2cbibyte[0] = i*2;
    i2cbibyte[1] = 1 << i;
    if (i2c_master_write_packet_wait(&i2c_master, &bibyte) != STATUS_OK)
      l("w not OK");  
    i2cbibyte[0] = i*2 + 1;
    i2cbibyte[1] = 1 << (8-i);
    if (i2c_master_write_packet_wait(&i2c_master, &bibyte) != STATUS_OK)
      l("w not OK");   }
  l("Draw stuff done");
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
  configure_pins();
  l("ui_init");
  ui_init();

  l("ui_powerdown");
  ui_powerdown();

  // Start USB stack to authorize VBus monitoring
  l("udc_start");
  udc_start();


  // configure_systick_handler();
  // system_interrupt_enable_global();
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

#ifdef USB_DEVICE_LPM_SUPPORT
void main_suspend_lpm_action(void)
{
  l("main_suspend_lpm_action");
  ui_powerdown();
}

void main_remotewakeup_lpm_disable(void)
{
  l("main_remotewakeup_lpm_disable");
  ui_wakeup_disable();
}

void main_remotewakeup_lpm_enable(void)
{
  l("main_remotewakeup_lpm_enable");
  ui_wakeup_enable();
}
#endif

bool main_cdc_enable(uint8_t port)
{
  l("main_cdc_enable %d", port);
  main_b_cdc_enable = true;
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


bool handle_pin_configuration_cb(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  l("Received a pin configuration callback");
  DonglePiRequest_Config_GPIO_Pin pin;
  if (!pb_decode(stream, DonglePiRequest_Config_GPIO_Pin_fields, &pin)) {
    l("Failed to decode a pin configuration");
  }
  l("Pin number %d", pin.number);
  l("Pin direction %d", pin.direction);
  
  struct port_config config_port_pin;
  port_get_config_defaults(&config_port_pin);
  if (pin.direction == DonglePiRequest_Config_GPIO_Pin_Direction_OUT) {
    config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
  } else {
    config_port_pin.direction = PORT_PIN_DIR_INPUT;
  }
  port_pin_set_config(pin_map[pin.number], &config_port_pin);
  return true;
}

bool handle_i2c_write_data_cb(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  l("Received a i2c write DATA callback");
  DonglePiData_I2C_Write* write = (DonglePiData_I2C_Write*)(*arg);
  l("Addr %02x", write->addr);
  size_t len = stream->bytes_left;
  l("Length %d", len);
  uint8_t buf[255];
  if (len > sizeof(buf) - 1 || !pb_read(stream, buf, len))
    return false;

  l("Data %02x %02x", buf[0], buf[1]);
  struct i2c_master_packet packet = {
      .address = write->addr,
      .data_length = len,
      .data = buf,
  };
  if (i2c_master_write_packet_wait(&i2c_master, &packet) != STATUS_OK)
    l("w not OK");  
 
  return true;
}

bool handle_i2c_write_cb(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  l("Received a i2c write callback");
  DonglePiData_I2C_Write write;
  write.buffer.funcs.decode = handle_i2c_write_data_cb;
  write.buffer.arg = &write;
  if (!pb_decode(stream, DonglePiData_I2C_Write_fields, &write)) {
    l("Failed to decode an I2C write");
  }
 
  return true;
}

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
  DonglePiRequest request = {{NULL}};
  request.config.gpio.pins.funcs.decode = handle_pin_configuration_cb;
  request.data.i2c.writes.funcs.decode = handle_i2c_write_cb;

  if (!pb_decode(&istream, DonglePiRequest_fields, &request)) {
    l("failed to decode the packet, wait for more data");
    return;
  }

  l("Request #%d received", request.message_nb);

  if(request.has_data && request.data.has_gpio) {
     l("Data received  mask = %x  values = %x", request.data.gpio.mask, request.data.gpio.values);
     for (uint32_t pin = 2; request.data.gpio.mask;  pin++) {
       uint32_t bit = 1 << pin;
       if (request.data.gpio.mask & bit) {
         request.data.gpio.mask ^= bit;
         bool value = request.data.gpio.values & bit; 
         l("Pin GPIO%02d set to %d", pin, value);
         port_pin_set_output_level(pin_map[pin], value);
       }
     }
  }


  pb_ostream_t ostream = pb_ostream_from_buffer(buffer, USB_BUFFER_SIZE);
  DonglePiResponse response = {};
  response.message_nb = request.message_nb;
  l("Create response for #%d", response.message_nb);
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

