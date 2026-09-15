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

# 보드 종류. 실제 키트에 들어있는 보드에 맞춘다.
#   pico  = Raspberry Pi Pico   (RP2040)  <- 현재 키트 (대부분)
#   pico2 = Raspberry Pi Pico 2 (RP2350)  <- 일부 섞여 있을 수 있음
# 강의안 §4.2는 프로젝트 생성 시 Board type을 "Pico"로 고르라고 하지만,
# 우리 키트는 Pico 2다. README 참고.
set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(lab_week1 C CXX ASM)

pico_sdk_init()

# 1주차는 프로그램이 하나뿐이다. 강의안 §4.3의 핵심 3줄이 그대로 보인다.
add_executable(lab1_blink
    lab1_blink.c
)

target_link_libraries(lab1_blink
    pico_stdlib
)

pico_add_extra_outputs(lab1_blink)

# printf는 9주차부터 쓰지만, 강의안 §4.2가 프로젝트 생성 시
# "Console over USB"를 켜두라고 하므로 맞춰 둔다.
pico_enable_stdio_usb(lab1_blink 1)
pico_enable_stdio_uart(lab1_blink 0)
