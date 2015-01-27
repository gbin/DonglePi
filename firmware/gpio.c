#include <asf.h>
#include <pb_decode.h>
#include "gpio.h"
#include "dbg.h"
#include "pins.h"
#include "protocol/donglepi.pb.h"

void handle_gpio_write(Data_GPIO gpio) {
  l("Data received  mask = %x  values = %x", gpio.mask, gpio.values);
  for (uint32_t pin = 2; gpio.mask; pin++) {
    uint32_t bit = 1 << pin;
    if (gpio.mask & bit) {
      gpio.mask ^= bit;
      bool value = gpio.values & bit;
      l("Pin GPIO%02d set to %d", pin, value);
      port_pin_set_output_level(pin_map[pin], value);
    }
  }
}

void handle_gpio_read(DonglePiResponse *response) {
  l("Read input pins");
  uint32_t mask = 0;
  uint32_t values = 0;
  for (int i = 0; i < 28; i++) {
    pinconfig_t *pin = get_pin_GPIO_config(i);
    if (pin->active && pin->direction == Config_GPIO_Pin_Direction_IN) {
      l("pin %d active and input", i);
      mask |= 1 << i;
      l("before values %x", values);
      values |= port_pin_get_input_level(pin_map[i]) << i;
      l("after values %x", values);
    }
  }
  if (mask) {
    response->has_data = true;
    response->data.has_gpio = true;
    response->data.gpio.mask = mask;
    response->data.gpio.values = values;
  }
}

bool handle_gpio_pin_config_cb(pb_istream_t *stream, const pb_field_t *field,
                               void **arg) {
  l("Received a pin configuration callback");
  Config_GPIO_Pin pin;
  if (!pb_decode(stream, Config_GPIO_Pin_fields, &pin)) {
    l("Failed to decode a pin configuration");
  }
  l("Pin active %d", pin.active);
  l("Pin number %d", pin.number);
  l("Pin direction %d", pin.direction);

  pinconfig_t config = {pin.active, pin.direction, pin.pull, pin.trigger};
  if (!set_pin_GPIO_config(pin.number, config)) {
    l("Error switching pin %d", pin.number);
    return false;
  }

  struct port_config config_port_pin;
  port_get_config_defaults(&config_port_pin);
  if (pin.direction == Config_GPIO_Pin_Direction_OUT) {
    config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
  } else {
    config_port_pin.direction = PORT_PIN_DIR_INPUT;
    if (pin.has_pull) {
      if (pin.pull == Config_GPIO_Pin_Pull_OFF) {
        config_port_pin.input_pull = PORT_PIN_PULL_NONE;
        l("Pull None");
      } else if (pin.pull == Config_GPIO_Pin_Pull_UP) {
        config_port_pin.input_pull = PORT_PIN_PULL_UP;
        l("Pull Up");
      } else if (pin.pull == Config_GPIO_Pin_Pull_DOWN) {
        config_port_pin.input_pull = PORT_PIN_PULL_DOWN;
        l("Pull Down");
      }
    } else {
      l("No pull config for this pin");
    }
  }
  port_pin_set_config(pin_map[pin.number], &config_port_pin);
  return true;
}
