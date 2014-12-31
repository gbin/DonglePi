# Path to top level ASF directory relative to this project directory.
PRJ_PATH = ../xdk

# Target CPU architecture: cortex-m3, cortex-m4
ARCH = cortex-m0plus

# Target part: none, sam3n4 or sam4l4aa
PART = samd21e17a

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET_FLASH = flash.elf
TARGET_SRAM = donglepi_sram.elf

# List of C source files.
CSRCS = \
       common/utils/interrupt/interrupt_sam_nvic.c        \
       donglepi.c \
			 lz4/lz4.c \
       sam0/drivers/extint/extint.c                       \
       sam0/drivers/extint/extint_callback.c              \
       sam0/drivers/port/port.c                           \
       sam0/drivers/system/clock/clock_samd21_r21/clock.c \
       sam0/drivers/system/clock/clock_samd21_r21/gclk.c  \
       sam0/drivers/system/interrupt/system_interrupt.c   \
       sam0/drivers/system/pinmux/pinmux.c                \
       sam0/drivers/system/system.c                       \
       sam0/utils/cmsis/samd21/source/gcc/startup_samd21.c \
       sam0/utils/cmsis/samd21/source/system_samd21.c     \
       sam0/utils/syscalls/gcc/syscalls.c

# List of assembler source files.
ASSRCS =

# List of include paths.
INC_PATH = \
       common/boards                                      \
       common/utils                                       \
       sam0/boards                                        \
       sam0/boards/dummy                                  \
       sam0/drivers/extint                                \
       sam0/drivers/port                                  \
       sam0/drivers/system                                \
       sam0/drivers/system/clock                          \
       sam0/drivers/system/clock/clock_samd21_r21         \
       sam0/drivers/system/interrupt                      \
       sam0/drivers/system/interrupt/system_interrupt_samd21 \
       sam0/drivers/system/pinmux                         \
       sam0/utils                                         \
       sam0/utils/cmsis/samd21/include                    \
       sam0/utils/cmsis/samd21/source                     \
       sam0/utils/header_files                            \
       sam0/utils/preprocessor                            \
       thirdparty/CMSIS/Include                           \
       thirdparty/CMSIS/Lib/GCC \
       sam0/applications/led_toggle/samd21_xplained_pro/gcc

# Additional search paths for libraries.
LIB_PATH = thirdparty/CMSIS/Lib/GCC

# List of libraries to use during linking.
LIBS = arm_cortexM0l_math

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT_FLASH = sam0/utils/linker_scripts/samd21/gcc/samd21j18a_flash.ld
LINKER_SCRIPT_SRAM  = sam0/utils/linker_scripts/samd21/gcc/samd21j18a_sram.ld

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
CFLAGS = -I.
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
       -D ARM_MATH_CM0=true                               \
       -D BOARD=DUMMY_BOARD                       \
       -D EXTINT_CALLBACK_MODE=true                       \
       -D __SAMD21E17A__

# Extra flags to use when linking
# gbin: stuff for the bootloader
LDFLAGS = -Wl,--section-start=.text=0x4000

# Pre- and post-build commands
PREBUILD_CMD =
POSTBUILD_CMD =
