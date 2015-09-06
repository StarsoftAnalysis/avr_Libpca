cmake_minimum_required(VERSION 2.8)
project (pca)


set(pca_VERSION_MAJOR 2)
set(pca_VERSION_MINOR 0)
set(pca_VERSION_PATCH 0)

configure_file("${PROJECT_SOURCE_DIR}/include/version/version.h.in"
    "${PROJECT_BINARY_DIR}/include/version/version.h" @ONLY)


set(CMAKE_C_COMPILER avr-gcc)
set(CSTANDARD " -std=gnu99")
set(CWARN " -Wall -Werror -Wstrict-prototypes")
set(CTUNING " -ffunction-sections -fdata-sections -fpack-struct -fno-tree-scev-cprop -fno-strict-aliasing -funsigned-bitfields -fno-inline-small-functions")
set(COPT " -Os")
set(CDEFS " -DF_CPU=16000000UL")
set(CMCU " -mmcu=atmega328p")

set(CFLAGS ${CSTANDARD}${CWARN}${CTUNING}${COPT}${CDEFS}${CMCU})
set(CMAKE_C_FLAGS ${CFLAGS})


file(GLOB SOURCES src/*.c)

add_library(pca STATIC ${SOURCES})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

install(TARGETS pca DESTINATION lib)
install(DIRECTORY include/
    DESTINATION include/libpca
    FILES_MATCHING PATTERN "*.h")

