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

# Select pico or pico2 for the target board.
set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(lab_week10 C CXX ASM)

pico_sdk_init()

# I2C scanner.
add_executable(lab1_i2c_scanner
    lab1_i2c_scanner.c
)
target_link_libraries(lab1_i2c_scanner
    pico_stdlib
    hardware_i2c
)

# I2C LCD with the LCD1602 driver.
add_executable(lab2_lcd_hello
    lab2_lcd_hello.c
    lcd1602.c
)
target_link_libraries(lab2_lcd_hello
    pico_stdlib
    hardware_i2c
)

add_executable(lab2b_oled_hello
    lab2b_oled_hello.c
    ssd1306.c
)
target_link_libraries(lab2b_oled_hello
    pico_stdlib
    hardware_i2c
)

# RC522 version register over SPI.
add_executable(lab3_rfid_version
    lab3_rfid_version.c
)
target_link_libraries(lab3_rfid_version
    pico_stdlib
    hardware_spi
)

# Card UID with the MFRC522 driver.
add_executable(lab3b_rfid_uid
    lab3b_rfid_uid.c
    mfrc522.c
)
target_link_libraries(lab3b_rfid_uid
    pico_stdlib
    hardware_spi
)

add_executable(lab5b_rfid_oled
    lab5b_rfid_oled.c
    mfrc522.c
    ssd1306.c
)
target_link_libraries(lab5b_rfid_oled
    pico_stdlib
    hardware_i2c
    hardware_spi
)

# Optional DHT11 example (disabled).
# add_executable(lab4_dht11
#     lab4_dht11.c
#     dht11.c
# )
# target_link_libraries(lab4_dht11
#     pico_stdlib
# )

# Optional RFID door example (disabled; requires hardware validation).
# add_executable(lab6_rfid_door
#     lab6_rfid_door.c
#     mfrc522.c
# )
# target_link_libraries(lab6_rfid_door
#     pico_stdlib
#     hardware_spi
#     hardware_pwm
# )

# USB console and build outputs.
foreach(lab lab1_i2c_scanner lab2_lcd_hello lab2b_oled_hello lab3_rfid_version lab3b_rfid_uid lab5b_rfid_oled)
    pico_enable_stdio_usb(${lab} 1)
    pico_enable_stdio_uart(${lab} 0)
    pico_add_extra_outputs(${lab})
endforeach()
