#ifndef _SPI_H_
#define _SPI_H_
#include <pb_decode.h>
#include "protocol/donglepi.pb.h"
bool handle_spi_config_cb(pb_istream_t *stream, const pb_field_t *field,
                          void **arg);
bool handle_spi_write_cb(pb_istream_t *stream, const pb_field_t *field,
                          void **arg);
#endif
