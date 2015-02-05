# Path to top level ASF directory relative to this project directory.
PRJ_PATH = ../asf

# Target CPU architecture: cortex-m3, cortex-m4
ARCH = cortex-m0plus

# Target part: none, sam3n4 or sam4l4aa
PART = samd21e17a

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET_FLASH = flash.elf
TARGET_SRAM = donglepi_sram.elf

#			 lz4/lz4.c \
# List of C source files.
CSRCS = \
       sam0/utils/cmsis/samd21/source/gcc/startup_samd21.c \
       sam0/utils/cmsis/samd21/source/system_samd21.c      \
       sam0/utils/syscalls/gcc/syscalls.c                  \
       common/services/sleepmgr/samd/sleepmgr.c            \
       common/services/usb/class/cdc/device/udi_cdc.c  \
       common/services/usb/class/cdc/device/udi_cdc_desc.c \
       common/services/usb/udc/udc.c                       \
       common/utils/interrupt/interrupt_sam_nvic.c         \
       common2/services/delay/sam0/cycle_counter.c         \
       sam0/drivers/extint/extint_sam_d_r/extint.c         \
       sam0/drivers/extint/extint_callback.c               \
       sam0/drivers/port/port.c                            \
       sam0/drivers/sercom/i2c/i2c_samd21_r21_d10_d11_l21/i2c_master.c \
       sam0/drivers/sercom/spi/spi.c    \
       sam0/drivers/sercom/usart/usart.c \
       sam0/drivers/sercom/sercom.c    \
       sam0/drivers/system/clock/clock_samd21_r21/clock.c  \
       sam0/drivers/system/clock/clock_samd21_r21/gclk.c   \
       sam0/drivers/system/interrupt/system_interrupt.c    \
       sam0/drivers/system/pinmux/pinmux.c                 \
       sam0/drivers/system/system.c                        \
       sam0/drivers/usb/stack_interface/usb_device_udd.c   \
       sam0/drivers/usb/stack_interface/usb_dual.c         \
			 sam0/drivers/usb/usb_sam_d_r/usb.c                              \
       nanopb/pb_encode.c																	 \
       nanopb/pb_decode.c																	 \
       nanopb/pb_common.c																	 \
			 protocol/donglepi.pb.c\
       donglepi.c                                          \
			 pins.c\
			 dbg.c\
			 i2c.c\
			 spi.c\
			 gpio.c\
			 uart.c


# List of assembler source files.
ASSRCS =

# List of include paths.
INC_PATH = \
       sam0/utils                                         \
       sam0/utils/cmsis/samd21/include                    \
       sam0/utils/cmsis/samd21/source                     \
       sam0/utils/header_files                            \
       sam0/utils/preprocessor                            \
       sam0/boards                                        \
       sam0/boards/dummy                                  \
       sam0/drivers/extint                                \
       sam0/drivers/sercom                                \
       sam0/drivers/port                                  \
       sam0/drivers/system/power/power_sam_d_r            \
			 sam0/drivers/system/reset/reset_sam_d_r            \
       sam0/drivers/sercom/usart                           \
       sam0/drivers/sercom/i2c                            \
       sam0/drivers/sercom/spi                            \
       sam0/drivers/system                                \
       sam0/drivers/system/clock                          \
       sam0/drivers/system/clock/clock_samd21_r21         \
       sam0/drivers/system/interrupt                      \
       sam0/drivers/system/interrupt/system_interrupt_samd21 \
       sam0/drivers/system/pinmux                         \
       sam0/drivers/usb \
       sam0/drivers/usb/stack_interface \
       common2/services/delay                             \
       common2/services/delay/sam0                        \
       common/boards                                      \
       common/services/sleepmgr                           \
       common/services/usb                                \
       common/services/usb/class/cdc                      \
       common/services/usb/class/cdc/device               \
       common/services/usb/udc                            \
       common/utils                                       \
       thirdparty/CMSIS/Include                           \
       thirdparty/CMSIS/Lib/GCC


# Additional search paths for libraries.
LIB_PATH = thirdparty/CMSIS/Lib/GCC

# List of libraries to use during linking.
LIBS = arm_cortexM0l_math

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT_FLASH = sam0/utils/linker_scripts/samd21/gcc/samd21e17a_flash.ld
LINKER_SCRIPT_SRAM  = sam0/utils/linker_scripts/samd21/gcc/samd21e17a_sram.ld

# Path relative to top level directory pointing to a linker script.
DEBUG_SCRIPT_FLASH = sam0/boards/samd21_xplained_pro/debug_scripts/gcc/samd21_xplained_pro_flash.gdb
DEBUG_SCRIPT_SRAM  = sam0/boards/samd21_xplained_pro/debug_scripts/gcc/samd21_xplained_pro_sram.gdb

# Project type parameter: all, sram or flash
PROJECT_TYPE        = flash

# Additional options for debugging. By default the common Makefile.in will
# add -g3.
DBGFLAGS =

# Application optimization used during compilation and linking:
# -O0, -O1, -O2, -O3 or -Os
OPTIMIZATION = -O1

# Extra flags to use when archiving.
ARFLAGS =

# Extra flags to use when assembling.
ASFLAGS =

# Extra flags to use when compiling.
CFLAGS = -I. -Inanopb
#
# Extra flags to use when preprocessing.
#
# Preprocessor symbol definitions
#   To add a definition use the format "-D name[=definition]".
#   To cancel a definition use the format "-U name".
#
# The most relevant symbols to define for the preprocessor are:
#   BOARD      Target board in use, see boards/board.h for a list.
#   EXT_BOARD  Optional extension board in use, see boards/board.h for a list.
CPPFLAGS = \
       -D ARM_MATH_CM0=true    \
       -D BOARD=DUMMY_BOARD \
       -D CONFIG_SLEEPMGR_ENABLE \
       -D SPI_CALLBACK_MODE=false \
			 -D CYCLE_MODE \
       -D USART_CALLBACK_MODE=false \
       -D EXTINT_CALLBACK_MODE=true  \
			 -D I2C_MASTER_CALLBACK_MODE=false \
       -D __SAMD21E17A__

# Extra flags to use when linking
# gbin: stuff for the bootloader
LDFLAGS = -Wl,--section-start=.text=0x4000

# Pre- and post-build commands
PREBUILD_CMD = make -C protocol
POSTBUILD_CMD =
