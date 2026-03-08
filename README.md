
![alt text](images/title.jpg)


# :car:  Project 3 <span style = "color : #8c04e7"> AutoMobility </span>

## **1. Project Summary (프로젝트 요약)**
STM32(MCU)를 활용하여 블루투스를 통한 수동조종(Manual) 및 자율주행(Auto) 시스템 제작


## 2. Key Features (주요 기능)

### 🕹️ Manual Mode (수동 제어)

- 조이스틱(Joystick)을 통해 차체를 조종가능
- PWM 신호를 통해 자동차의 속도를 변경 가능하고 이를 조이스틱 감도로 제어가능

### 🤖 Auto Mode (자율주행)

- 센서(Ultrasonic) 데이터를 기반으로 장애물 회피
- 데이터를 이중으로 비교하여 회전 중에도 재판단
- 코너에 진입했는데 전면과의 거리가 너무 가까우면 넓은 방향으로 후진

### 🛠 기술 스택 (Tech Stack)

| Language | IDE | Configuration |
| :---: | :---: | :---: |
| <img src="https://img.icons8.com/color/48/000000/c-programming.png" width="50"> | ![alt text](images/stm32cubeide.png) | ![alt text](images/stm32cubemx.png) |
| **C Language** | **STM32CubeIDE** | **STM32CubeMX** |

![alt text](images/stm32cubeide.png)
![alt text](images/stm32cubemx.png)
![STM32](https://img.shields.io/badge/STM32-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white)
Language: C
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
MCU: STM32F411RETx (HAL Driver 기반)

Motor Driver: L298N Dual H-Bridge

IDE: STM32CubeIDE

Version Control: Git

📂 프로젝트 구조 (Project Structure)
Plaintext
RC_CAR_R02/
├── Core/
│   ├── Src/            # 주요 제어 로직 및 인터럽트 핸들러
│   └── Inc/            # 헤더 파일
├── Drivers/
│   └── STM32F4xx_HAL_Driver  # HAL 라이브러리
└── RC_CAR_R02.ioc      # CubeMX 설정 파일
🔌 하드웨어 연결 (Hardware Setup)
Motor Driver (L298N):

IN1, IN2: Left Motor Control

IN3, IN4: Right Motor Control

ENA, ENB: PWM Speed Control

Power: 외부 전원 공급 장치를 통한 모터 전원 분리

![BlockDiagram](images/Project3_Automobility_BlockDiagram.png)
![alt text](images/Project3_Automobility_Statemachine.png)

🏁 시작하기 (Getting Started)
이 저장소를 클론합니다.

Bash
git clone https://github.com/your-username/RC_CAR_R02.git
STM32CubeIDE에서 프로젝트를 임포트합니다.

빌드 후 보드에 Flash하여 동작을 확인합니다.