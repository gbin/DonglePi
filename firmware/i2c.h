#ifndef _I2C_H_
#define _I2C_H_
#include <pb_decode.h>
#include "protocol/donglepi.pb.h"
bool handle_i2c_config_cb(pb_istream_t *stream, const pb_field_t *field,
                          void **arg);
bool handle_i2c_write_cb(pb_istream_t *stream, const pb_field_t *field,
                         void **arg);
#endif
