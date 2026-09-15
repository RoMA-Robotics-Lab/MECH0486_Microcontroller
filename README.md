# MECH0486 마이크로컨트롤러

RoMA Robotics Lab 마이크로컨트롤러 수업 실습 코드.
Raspberry Pi Pico와 Pico SDK(C/C++)를 사용한다.

## 실습

- `lab_week1`: 온보드 LED 깜빡이기
- `lab_week2`: LED 순차 점등, 버튼 입력, USB 시리얼 출력

배선은 각 소스 파일 상단의 주석 참고.

## 사용 방법

저장소를 내려받고, VS Code의 Raspberry Pi Pico 확장에서 실습할 주차 폴더를 가져온다.
저장소 전체가 아니라 `CMakeLists.txt`가 있는 `lab_week1`, `lab_week2` 폴더를 각각 선택한다.

보드 설정은 기본값이 `pico`다. Pico 2를 사용하면 `pico2`로 바꾼다.
빌드 후 BOOTSEL 버튼을 누른 채 보드를 USB에 연결하고, `build` 폴더의 해당 실습 `.uf2` 파일을 보드 드라이브에 복사한다.

2주차 시리얼 예제는 업로드 후 시리얼 모니터에서 보드의 COM 포트를 열면 된다.
`build` 폴더는 각 PC에서 생성하므로 저장소에 포함하지 않는다.
