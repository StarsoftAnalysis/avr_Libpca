cmake_minimum_required(VERSION 2.8)
project (pca)


set(PCA_VERSION_MAJOR 2)
set(PCA_VERSION_MINOR 0)
set(PCA_VERSION_PATCH 0)

set(PCA_VERSION ${PCA_VERSION_MAJOR}.${PCA_VERSION_MINOR})
set(PCA_VERSION_DETAILED ${PCA_VERSION_MAJOR}.${PCA_VERSION_MINOR}.${PCA_VERSION_PATCH})

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


set(PCA_INCLUDE_SUFFIX "include/pca-${PCA_VERSION}")
set(PCA_LIBRARY_NAME "pca-${PCA_VERSION_DETAILED}")

file(GLOB_RECURSE SOURCES src/*.c)
add_library(${PCA_LIBRARY_NAME} STATIC ${SOURCES})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

install(TARGETS ${PCA_LIBRARY_NAME} DESTINATION lib)
install(DIRECTORY include/
    DESTINATION ${PCA_INCLUDE_SUFFIX}
    FILES_MATCHING PATTERN "*.h")

set(PCA_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(PCA_LIBRARIES_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(PCA_LIBRARY ${PCA_LIBRARY_NAME})
configure_file(PcaConfig.cmake.in "${PROJECT_BINARY_DIR}/PcaConfig.cmake" @ONLY)
configure_file(PcaConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/PcaConfigVersion.cmake" @ONLY)
install(FILES
    PcaConfig.cmake
    PcaConfigVersion.cmake
    DESTINATION "lib/cmake/pca-${PCA_VERSION}"
    COMPONENT dev)
