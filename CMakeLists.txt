cmake_minimum_required( VERSION 2.0.0)
project (libpca)

set(libpca_VERSION_MAJOR 2)
set(libpca_VERSION_MINOR 0)

set(CMAKE_C_COMPILER avr-gcc)
set(CSTANDARD " -std=gnu99")
set(CWARN " -Wall -Werror -Wstrict-prototypes")
set(CTUNING " -ffunction-sections -fdata-sections -fpack-struct -fno-tree-scev-cprop -fno-strict-aliasing -funsigned-bitfields -fno-inline-small-functions")
set(COPT " -Os")
set(CDEFS " -DF_CPU=16000000UL")
set(CMCU " -mmcu=atmega328p")

set(CFLAGS ${CSTANDARD}${CWARN}${CTUNING}${COPT}${CDEFS}${CMCU})
set(CMAKE_C_FLAGS ${CFLAGS})


add_subdirectory(src)
