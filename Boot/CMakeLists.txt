cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32F103C8T6_Template)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)


# ============================================================
# Convert ELF to HEX / BIN


# ============================================================
set(TARGET_ELF "${CMAKE_PROJECT_NAME}.elf")
set(TARGET_HEX "${CMAKE_PROJECT_NAME}.hex")
set(TARGET_BIN "${CMAKE_PROJECT_NAME}.bin")

# arm-none-eabi-objcopy must be in PATH (declared in toolchain file)
add_custom_command(
    OUTPUT  ${TARGET_HEX} ${TARGET_BIN}
    COMMAND ${CMAKE_OBJCOPY} -O ihex   ${TARGET_ELF} ${TARGET_HEX}
    COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET_ELF} ${TARGET_BIN}
    DEPENDS ${CMAKE_PROJECT_NAME}
    COMMENT "Converting ${TARGET_ELF} -> ${TARGET_HEX} / ${TARGET_BIN}"
    VERBATIM
)

add_custom_target(hex
    DEPENDS ${TARGET_HEX} ${TARGET_BIN}
    COMMENT "Generate ${TARGET_HEX} and ${TARGET_BIN}"
)