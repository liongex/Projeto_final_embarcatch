# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Projeto_final_embarcatch C CXX ASM)


# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(dht)

# Add executable. Default name is the project name, version 0.1

add_executable(Projeto_final_embarcatch 
    Projeto_final_embarcatch.c 
    pwm_motor.c
    ssd1306.c 
    display.c 
    temperatura.c

)

pico_set_program_name(Projeto_final_embarcatch "Projeto_final_embarcatch")
pico_set_program_version(Projeto_final_embarcatch "0.1")

# Generate PIO header
pico_generate_pio_header(Projeto_final_embarcatch ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Projeto_final_embarcatch 1)
pico_enable_stdio_usb(Projeto_final_embarcatch 1)

# Add the standard library to the build
target_link_libraries(Projeto_final_embarcatch
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(Projeto_final_embarcatch PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_options(Projeto_final_embarcatch  PRIVATE -Wall)

# Add any user requested libraries
target_link_libraries(Projeto_final_embarcatch 
        hardware_pio
        dht
        hardware_pwm
        hardware_i2c
        hardware_adc
        pico_stdlib
        pico_lwip_http
        pico_cyw43_arch_lwip_threadsafe_background  
        )

pico_add_extra_outputs(Projeto_final_embarcatch)

