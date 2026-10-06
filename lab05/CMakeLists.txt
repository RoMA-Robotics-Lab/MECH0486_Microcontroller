cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(lab_week5 C CXX ASM)

pico_sdk_init()

set(LABS
    lab1_clock_check
    lab2_clock_sleep
    lab3_clock_timer
    lab4_alarm_callback
)

set(LABS_SEG
    lab5_seg_clock
    lab6_clock_button
    lab7_hourglass
)

foreach(lab IN LISTS LABS)
    add_executable(${lab} ${lab}.c)

    target_link_libraries(${lab} pico_stdlib)

    pico_enable_stdio_usb(${lab} 1)
    pico_enable_stdio_uart(${lab} 0)

    pico_add_extra_outputs(${lab})
endforeach()

#   add_executable(lab5_seg_clock  lab5_seg_clock.c  tm1637.c)
foreach(lab IN LISTS LABS_SEG)
    add_executable(${lab} ${lab}.c tm1637.c)

    target_link_libraries(${lab} pico_stdlib)

    pico_enable_stdio_usb(${lab} 1)
    pico_enable_stdio_uart(${lab} 0)

    pico_add_extra_outputs(${lab})
endforeach()
