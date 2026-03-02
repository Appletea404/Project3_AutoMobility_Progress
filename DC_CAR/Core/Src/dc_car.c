#include "dc_car.h"

// main.c에 선언된 수신 데이터 배열 크기 일치 (1바이트)
extern volatile uint8_t rxData[1];

// 전진: 양쪽 모터를 정방향으로 회전
void GO_UP()
{
    HAL_GPIO_WritePin(DC_PORT_IN1, DC_PIN_IN1, GPIO_PIN_SET);   // IN1 (PC9)
    HAL_GPIO_WritePin(DC_PORT_IN2, DC_PIN_IN2, GPIO_PIN_RESET); // IN2 (PB8)
    HAL_GPIO_WritePin(DC_PORT_IN3, DC_PIN_IN3, GPIO_PIN_SET);   // IN3 (PC8)
    HAL_GPIO_WritePin(DC_PORT_IN4, DC_PIN_IN4, GPIO_PIN_RESET); // IN4 (PC6)
}

// 후진: 양쪽 모터를 역방향으로 회전
void GO_DOWN()
{
    HAL_GPIO_WritePin(DC_PORT_IN1, DC_PIN_IN1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT_IN2, DC_PIN_IN2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DC_PORT_IN3, DC_PIN_IN3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT_IN4, DC_PIN_IN4, GPIO_PIN_SET);
}

// 좌회전: 제자리 좌회전
void GO_LEFT()
{
    HAL_GPIO_WritePin(DC_PORT_IN1, DC_PIN_IN1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT_IN2, DC_PIN_IN2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DC_PORT_IN3, DC_PIN_IN3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DC_PORT_IN4, DC_PIN_IN4, GPIO_PIN_RESET);
}

// 우회전: 제자리 우회전
void GO_RIGHT()
{
    HAL_GPIO_WritePin(DC_PORT_IN1, DC_PIN_IN1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DC_PORT_IN2, DC_PIN_IN2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT_IN3, DC_PIN_IN3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT_IN4, DC_PIN_IN4, GPIO_PIN_SET);
}

// 정지: 모든 출력을 Low
void GO_STOP()
{
    HAL_GPIO_WritePin(DC_PORT_IN1, DC_PIN_IN1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT_IN2, DC_PIN_IN2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT_IN3, DC_PIN_IN3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT_IN4, DC_PIN_IN4, GPIO_PIN_RESET);
}

// 블루투스로 받은 문자에 따라 모터 제어 (switch-case로 최적화)
void DC_CONTROL(uint8_t cmd)
{
    switch(cmd)
    {
        case 'F':
            GO_UP();
            break;
        case 'B':
            GO_DOWN();
            break;
        case 'L':
            GO_LEFT();
            break;
        case 'R':
            GO_RIGHT();
            break;
        case 'S':
            GO_STOP();
            break;
        default:
            break;
    }
}

// 초기 테스트용 함수
void DC_RUN()
{
    GO_UP();
}