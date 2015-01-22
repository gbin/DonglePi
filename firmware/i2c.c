#include <asf.h>
#include "dbg.h"
#include "i2c.h"
#include "pins.h"

struct i2c_master_module i2c_master;

static void i2c_config(Config_I2C i2c_proto_config) {
  struct i2c_master_config config_i2c_master;
  i2c_master_get_config_defaults(&config_i2c_master);
  config_i2c_master.buffer_timeout = 10000;
  config_i2c_master.pinmux_pad0 = PINMUX_PA16C_SERCOM1_PAD0;
  config_i2c_master.pinmux_pad1 = PINMUX_PA17C_SERCOM1_PAD1;
  if (i2c_proto_config.speed == Config_I2C_Speed_BAUD_RATE_100KHZ) {
    config_i2c_master.baud_rate = I2C_MASTER_BAUD_RATE_100KHZ;
  } else {
    config_i2c_master.baud_rate = I2C_MASTER_BAUD_RATE_400KHZ;
  }

  if (i2c_master_init(&i2c_master, SERCOM1, &config_i2c_master)!=STATUS_OK) {
    l("I2C Init Error");
  }
  i2c_master_enable(&i2c_master);
  l("I2C enabled");
}

static void i2c_disable(void) {
  i2c_master_disable(&i2c_master);
  l("I2C disabled");
}


bool handle_i2c_config_cb(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    l("Configuration for i2c...");
    Config_I2C i2c;
    if (!pb_decode(stream, Config_I2C_fields, &i2c)) {
      l("Failed to decode a i2c configuration");
    }

    if (switch_i2c(i2c.enabled)) {
      if (i2c.enabled) {
        i2c_config(i2c);
      } else {
        i2c_disable();
      }
    } else {
      l("I2C cannot be enabled/disabled");
    }
    return true;
}

static bool handle_i2c_write_data_cb(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  l("Received a i2c write DATA callback");
  Data_I2C_Write* write = (Data_I2C_Write*)(*arg);
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
  Data_I2C_Write write;
  write.buffer.funcs.decode = handle_i2c_write_data_cb;
  write.buffer.arg = &write;
  if (!pb_decode(stream, Data_I2C_Write_fields, &write)) {
    l("Failed to decode an I2C write");
  }
  return true;
}


