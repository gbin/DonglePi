#ifndef _I2C_H_
#define _I2C_H_
#include <pb_decode.h>
#include "protocol/donglepi.pb.h"
void handle_i2c_config(Config config_pb);
bool handle_i2c_write_cb(pb_istream_t *stream, const pb_field_t *field, void **arg);
#endif

