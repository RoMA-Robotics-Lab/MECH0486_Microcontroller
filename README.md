# MECH0486 Microcontroller

RoMA Robotics Lab의 마이크로컨트롤러 수업 실습 코드입니다. Raspberry Pi Pico와 Pico SDK(C/C++)를 사용합니다.

## 실습 구성

| 폴더 | 내용 |
| --- | --- |
| `lab_week1` | 온보드 LED 깜빡이기 (Blink) |
| `lab_week2` | LED 3개 순차 점등, 버튼 입력, USB 시리얼 출력 |

## 시작하기

1. 저장소를 다운로드하거나 복제합니다.
   ```sh
   git clone https://github.com/RoMA-Robotics-Lab/MECH0486_Microcontroller.git
   ```
2. VS Code와 Raspberry Pi Pico 확장을 준비합니다.
3. 실습할 주차 폴더(`lab_week1` 또는 `lab_week2`)를 Pico 프로젝트로 가져옵니다. 각 폴더에 `CMakeLists.txt`가 있습니다.
4. 보드 종류를 확인하고 빌드합니다. 현재 기본 설정은 `pico`이며, Pico 2를 사용하면 `pico2`로 설정합니다.
5. 보드의 **BOOTSEL** 버튼을 누른 채 USB에 연결하고, `build` 폴더에 생성된 해당 실습의 `.uf2` 파일을 보드 드라이브에 복사합니다.

배선과 예제별 설명은 각 `.c` 파일의 주석을 참고하세요. 2주차 시리얼 예제는 업로드 후 보드의 COM 포트를 시리얼 모니터로 열어 확인합니다.

## 파일 관리

- `build/`는 자동 생성되는 폴더로 Git에 포함하지 않습니다.
- `CMakeLists.txt`의 주석은 영문으로 작성합니다.
- 실습을 추가할 때는 해당 주차의 소스 파일과 `CMakeLists.txt`를 함께 갱신합니다.
