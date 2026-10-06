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
set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(lab_week3 C CXX ASM)

pico_sdk_init()

# 설정이 모두 같으므로 목록으로 처리한다.
# (강의안 §6.2 템플릿의 add_executable / target_link_libraries /
#  pico_add_extra_outputs 세 줄이 아래 반복문 안에 그대로 들어 있다.)
set(LABS
    lab1_flow_basic       # 6.1 기본 - 흐르는 불빛 (배열로 하나씩)
    lab2_flow_mask        # 6.2 변주1 - 같은 동작을 마스크/시프트로
    lab3_counter          # 6.3 변주2 - 3비트 이진 카운터
    ref_reg_peek          # 6.2 심화 - 레지스터 값을 printf로 관찰 (참고자료)
)

# 강의안 6.4(왕복)와 6.5(버튼 패턴)는 실습노트에서 '문제'로 낸다.
# 학생이 직접 작성하므로 여기에는 없다. 정답은 solution_week3/ 참고.
#
# 학생이 자기 코드를 빌드하려면 위 LABS 목록에 파일 이름을 추가하면 된다.
#   예)  my_knight        # 내가 만든 왕복 불빛

foreach(lab IN LISTS LABS)
    add_executable(${lab} ${lab}.c)

    # GPIO만 쓰므로 pico_stdlib 하나로 충분하다.
    # 7주차에 PWM/ADC를 쓰게 되면 hardware_pwm, hardware_adc를 추가한다.
    target_link_libraries(${lab} pico_stdlib)

    pico_enable_stdio_usb(${lab} 1)
    pico_enable_stdio_uart(${lab} 0)

    pico_add_extra_outputs(${lab})
endforeach()
