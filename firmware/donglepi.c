#include <asf.h>
#include "board.h"
#include "lz4/lz4.h"

/** Updates the board LED to the current button state. */
static void update_led_state(void)
{
  // gbin: BUTTON_A is connected to A27
  bool pin_state = port_pin_get_input_level(PIN_PA27);
  // gbin: LED is A28
  port_pin_set_output_level(PIN_PA28, pin_state);
}

/** Handler for the device SysTick module, called when the SysTick counter
 *  reaches the set period.
 *
 *  \note As this is a raw device interrupt, the function name is significant
 *        and must not be altered to ensure it is hooked into the device's
 *        vector table.
 */
void SysTick_Handler(void)
{
  update_led_state();
}

/** Configures the SysTick module to fire a SysTick interrupt every 999 system
 *  clock source cycles.
 */
static void configure_systick_handler(void)
{
  SysTick->CTRL = 0;
  SysTick->LOAD = 999;
  SysTick->VAL  = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


int main(void)
{
  system_init();


  configure_systick_handler();
  system_interrupt_enable_global();

  const char *source = "This is my input !";
  char dest[200];
  char restored[17];
  LZ4_compress (source, dest, 17);
  LZ4_decompress_fast(dest, restored, 17);
  while (true) {
    /* Do nothing - use interrupts */
  }
}
