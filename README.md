
![alt text](images/title.jpg)


# :car:  **Project 3 <span style = "color : #8c04e7"> AutoMobility </span>**
---

## **1. Project Summary (프로젝트 요약)**
---
STM32(MCU)를 활용하여 블루투스를 활용한 수동조종(Manual) 및 자율주행(Auto) 시스템 제작


## **2. Key Features (주요 기능)**

### 🕹️ **Manual Mode (수동 제어)**

- 조이스틱을 통해 
PWM 신호를 통한 정밀한 속도 조절이 가능합니다.

센서 데이터를 기반으로 장애물을 회피하며 스스로 주행합니다.
실시간 데이터 처리를 통해 주행 경로를 판단합니다.

🛠 기술 스택 (Tech Stack)
Language: C

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


🏁 시작하기 (Getting Started)
이 저장소를 클론합니다.

Bash
git clone https://github.com/your-username/RC_CAR_R02.git
STM32CubeIDE에서 프로젝트를 임포트합니다.

빌드 후 보드에 Flash하여 동작을 확인합니다.