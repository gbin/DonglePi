#include <asf.h>
#include "conf_usb.h"
#include "board.h"
#include "ui.h"
#include "uart.h"
#include "dbg.h"

static volatile bool main_b_cdc_enable = false;


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

void cdc_config(uint8_t port, usb_cdc_line_coding_t * cfg) {
 	l("cdc_config [%d]", port);
}

void cdc_rx_notify(uint8_t port) {
 	l("cdc_rx_notify [%d]", port);
 	l("data [%s]", udi_cdc_getc());
}

/* compression test 
  // const char *source = "This is my input !";
  //char dest[200];
  //char restored[17];
  //LZ4_compress (source, dest, 17);
  //LZ4_decompress_fast(dest, restored, 17);
*/

