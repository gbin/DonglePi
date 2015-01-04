#include <asf.h>
#include "conf_usb.h"
#include "board.h"
#include "ui.h"
#include "uart.h"

static volatile bool main_b_cdc_enable = false;

struct usart_module usart_module;
struct usart_config usart_conf;


/* void SysTick_Handler(void) {
  // gbin: BUTTON_A is connected to A27
  bool pin_state = port_pin_get_input_level(PIN_PA27);
  // gbin: LED is A28
  port_pin_set_output_level(PIN_PA28, pin_state);
}*/

/* static void configure_systick_handler(void) {
  SysTick->CTRL = 0;
  SysTick->LOAD = 999;
  SysTick->VAL  = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}*/


static void on1(void) {
  port_pin_set_output_level(PIN_PA28, 1);
}

static void off1(void) {
  port_pin_set_output_level(PIN_PA28, 0);
}

static void on2(void) {
  port_pin_set_output_level(PIN_PA02, 1);
}

static void off2(void) {
  port_pin_set_output_level(PIN_PA02, 0);
}

static void configure_pins(void) {
  struct port_config config_port_pin;
  port_get_config_defaults(&config_port_pin);
  config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
  port_pin_set_config(PIN_PA28, &config_port_pin);
  port_pin_set_config(PIN_PA02, &config_port_pin);
  
 }

static void configure_usart(void) {
  usart_get_config_defaults(&usart_conf);
  usart_conf.stopbits = USART_STOPBITS_1;
  usart_conf.parity = USART_PARITY_NONE;
  usart_conf.character_size = USART_CHARACTER_SIZE_8BIT;

	usart_conf.baudrate = 115200; 
	usart_conf.mux_setting = USART_RX_1_TX_0_XCK_1;
	usart_conf.pinmux_pad0 = PINMUX_PA16D_SERCOM3_PAD0;
	usart_conf.pinmux_pad1 = PINMUX_PA17D_SERCOM3_PAD1;
	usart_conf.pinmux_pad2 = PINMUX_UNUSED;
	usart_conf.pinmux_pad3 = PINMUX_UNUSED;
  on1();
	while (usart_init(&usart_module, SERCOM3, &usart_conf) != STATUS_OK) {
  }
  off1();
  off2();
  on2();
  usart_enable(&usart_module);
}


int main(void)
{
 	// irq_initialize_vectors();
	// cpu_irq_enable();
	// sleepmgr_init();
  system_init();
  configure_usart();
  configure_pins();
  ui_init();
  uint8_t string[] = "DonglePi debug console\n";

	// ui_powerdown();

	// Start USB stack to authorize VBus monitoring
	// udc_start();


  // configure_systick_handler();
  // system_interrupt_enable_global();
  on2();
  on1();
  usart_write_buffer_wait(&usart_module, string, sizeof(string));

  while (true) {
   // sleepmgr_enter_sleep();
  }
}

void main_suspend_action(void) {
	ui_powerdown();
}

void main_resume_action(void) {
	ui_wakeup();
}

void main_sof_action(void)
{
	if (!main_b_cdc_enable)
		return;
	// ui_process(udd_get_frame_number());
}

#ifdef USB_DEVICE_LPM_SUPPORT
void main_suspend_lpm_action(void)
{
	ui_powerdown();
}

void main_remotewakeup_lpm_disable(void)
{
	ui_wakeup_disable();
}

void main_remotewakeup_lpm_enable(void)
{
	ui_wakeup_enable();
}
#endif

bool main_cdc_enable(uint8_t port)
{
	main_b_cdc_enable = true;
	return true;
}

void main_cdc_disable(uint8_t port)
{
	main_b_cdc_enable = false;
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
/*	if (b_enable) {
		// Host terminal has open COM
		ui_com_open(port);
	}else{
		// Host terminal has close COM
		ui_com_close(port);
	}*/
}

void ui_powerdown(void) {
  // port_pin_set_output_level(PIN_PA28, 0);
}

void ui_init(void) {
/*
#ifdef USB_DEVICE_LPM_SUPPORT
  struct extint_chan_conf config_extint_chan;

  extint_chan_get_config_defaults(&config_extint_chan);

  config_extint_chan.gpio_pin            = BUTTON_0_EIC_PIN;
  config_extint_chan.gpio_pin_mux        = BUTTON_0_EIC_MUX;
  config_extint_chan.gpio_pin_pull       = EXTINT_PULL_UP;
  config_extint_chan.filter_input_signal = true;
  config_extint_chan.detection_criteria  = EXTINT_DETECT_FALLING;
  extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
  extint_register_callback(ui_wakeup_handler, BUTTON_0_EIC_LINE,
      EXTINT_CALLBACK_TYPE_DETECT);
  extint_chan_enable_callback(BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
#endif
*/
  /* Initialize LEDs */
}
void ui_wakeup(void)
{
  // port_pin_set_output_level(PIN_PA28, 1);
}

/* compression test 
  // const char *source = "This is my input !";
  //char dest[200];
  //char restored[17];
  //LZ4_compress (source, dest, 17);
  //LZ4_decompress_fast(dest, restored, 17);
*/

