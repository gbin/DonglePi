#ifndef _PINS_H_
#define _PINS_H_

// everything related to pin configuration.
extern uint8_t pin_map[28];

typedef struct pinconfig {
  bool active;
  Config_GPIO_Pin_Direction direction;
  Config_GPIO_Pin_Pull pull;
  Config_GPIO_Pin_Edge trigger;
} pinconfig_t;


#define SPI_PIN1 8
#define SPI_PIN2 9
#define SPI_PIN3 10
#define SPI_PIN4 11

#define UART_PIN1 14
#define UART_PIN2 15

#define I2C_PIN1 2
#define I2C_PIN2 3

#define is_spi_pin(X) (X==SPI_PIN1 || X==SPI_PIN2 || X==SPI_PIN3 || X==SPI_PIN4)
#define is_uart_pin(X) (X==UART_PIN1 || X==UART_PIN2)
#define is_i2c_pin(X) (X==I2C_PIN1 || X==I2C_PIN2)

bool is_available_for_GPIO(uint8_t pin);
bool set_pin_GPIO_config(uint8_t pin, pinconfig_t config);

bool switch_i2c(bool on);
bool switch_spi(bool on);
bool switch_uart(bool on);

#endif
