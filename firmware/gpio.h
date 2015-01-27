#ifndef _GPIO_H_
#define _GPIO_H_
#include "protocol/donglepi.pb.h"
void handle_gpio_write(Data_GPIO gpio);
bool handle_gpio_pin_config_cb(pb_istream_t *stream, const pb_field_t *field,
                               void **arg);
void handle_gpio_read(DonglePiResponse *response);
#endif
