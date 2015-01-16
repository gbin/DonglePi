#include "protocol/donglepi.pb.h"
#include "pins.h"
#include "board.h"

// RPI GPIO # -> SAMD pin#
uint8_t pin_map[28] = {
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

static pinconfig_t pin_configs[28] = { 0 };

static bool i2c_on = false;
static bool uart_on = true;
static bool spi_on = false;

// returns true if the pin is assignable to a GPIO
bool is_available_for_GPIO(uint8_t pin) {
  return (pin<28 && pin_map[pin] != 0) &&
        !(i2c_on && is_uart_pin(pin)) &&
        !(uart_on && is_uart_pin(pin)) &&
        !(spi_on && is_spi_pin(pin));
}

bool set_pin_GPIO_config(uint8_t pin, pinconfig_t config) {
  if (config.active && !is_available_for_GPIO(pin)) {
    return false;
  }
  pin_configs[pin] = config;
  return true;
}

bool switch_i2c(bool on) {
  if (on &&
      (pin_configs[I2C_PIN1].active ||
      pin_configs[I2C_PIN2].active)) {
      return false;
  }
  i2c_on = on;
  return true;
}

bool switch_spi(bool on) {
  if (on &&
      (pin_configs[SPI_PIN1].active ||
      pin_configs[SPI_PIN2].active ||
      pin_configs[SPI_PIN3].active ||
      pin_configs[SPI_PIN4].active)) {
      return false;
  }
  spi_on = on;
  return true;
}

bool switch_uart(bool on) {
  if (on &&
      (pin_configs[UART_PIN1].active ||
      pin_configs[UART_PIN2].active)) {
      return false;
  }
  uart_on = on;
  return true;
}

