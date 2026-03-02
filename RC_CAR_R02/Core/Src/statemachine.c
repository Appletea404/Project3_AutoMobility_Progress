/*
 * statemachine.c
 *
 *  Created on: Mar 1, 2026
 *      Author: appletea
 */

#include "statemachine.h"




extern UART_HandleTypeDef huart1;
static volatile uint8_t rxData[1];
static volatile uint8_t rxFlag = 0;
static uint8_t rxCmd = 0;
static AUTO_STATE auto_st = AUTO_STATE_STOP;
static uint32_t auto_tick = 0;


void STMACHINE_Init(void)
{
    // UART1 수신 인터럽트 시작
    HAL_UART_Receive_IT(&huart1, (uint8_t *)rxData, 1);
}

// UART 수신 콜백 함수를 여기로 이동
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        rxCmd = rxData[0];
        rxFlag = 1;
        // 다음 수신 대기
        HAL_UART_Receive_IT(&huart1, (uint8_t *)rxData, 1);
    }
}




static void DC_CONTROL_MANUAL(uint8_t cmd)
{
    switch(cmd)
    {
        case 'F':
        	Car_Move(CAR_FRONT, SPD_100);
            break;
        case 'B':
        	Car_Move(CAR_BACK, SPD_100);
            break;
        case 'L':
        	Car_Move(CAR_LEFT, SPD_100);
            break;
        case 'R':
        	Car_Move(CAR_RIGHT, SPD_100);
            break;
        case 'S':
        	Car_Stop();
        	break;
        default:
            break;
    }
}


const static uint32_t waitTick = 200;
static uint32_t prevTick = 0;


static void SHOW_UART2()
{
	uint32_t currentTick = HAL_GetTick();
		if ((currentTick - prevTick) < waitTick) return; // 200ms 아직 안 됨
		prevTick = currentTick;
	printf("LEFT : %d cm\r\n CENTER : %d cm\r\n RIGHT : %d cm\r\n",
			Ultrasonic_GetDistanceCm(US_LEFT),
			Ultrasonic_GetDistanceCm(US_CENTER),
			Ultrasonic_GetDistanceCm(US_RIGHT));
}

void DC_CONTROL_AUTO() {


	uint32_t current_Tick = HAL_GetTick();

	switch (auto_st) {
		case AUTO_STATE_SCAN:

			Ultrasonic_TriggerAll();
			uint8_t DisLeft = Ultrasonic_GetDistanceCm(US_LEFT);
			uint8_t DisCenter = Ultrasonic_GetDistanceCm(US_CENTER);
			uint8_t DisRight = Ultrasonic_GetDistanceCm(US_RIGHT);

			if (DisCenter > 0 && DisCenter < 30)
			{
				Car_Stop();
				auto_tick = current_Tick;
				auto_st = AUTO_STATE_STOP; // '잠시 멈춤' 단계로 전이
			}
			else if (DisLeft > 0 && DisLeft < 20)
				Car_Move(CAR_RIGHT, SPD_30);
			else if (DisRight > 0 && DisRight < 20)
				Car_Move(CAR_LEFT, SPD_30);
			else
				Car_Move(CAR_FRONT, SPD_30);
			break;

		case AUTO_STATE_STOP:
			if (current_Tick - auto_tick >= 100) {
				// 왼쪽/오른쪽 판단 후 회전 시작
				if (Ultrasonic_GetDistanceCm(US_LEFT) > Ultrasonic_GetDistanceCm(US_RIGHT))
				{
					Car_Move(CAR_LEFT, SPD_50);
				}
				else
				{
					Car_Move(CAR_RIGHT, SPD_50);
				}
				auto_tick = current_Tick;
				auto_st = AUTO_STATE_AVOID; // '회전 유지' 단계로 전이
			}
		break;

		case AUTO_STATE_AVOID:
			if (current_Tick - auto_tick >= 300)
			{
				auto_st = AUTO_STATE_SCAN; // 다시 감시 모드로 복귀
			}
			break;

		default:
			break;
	}







//	uint8_t DisLeft = Ultrasonic_GetDistanceCm(US_LEFT);
//	uint8_t DisCenter = Ultrasonic_GetDistanceCm(US_CENTER);
//	uint8_t DisRight = Ultrasonic_GetDistanceCm(US_RIGHT);
//
//	 // 0은 측정 오류일 경우가 많으므로 큰 값으로 치환하여 무시
//	    if (DisCenter == 0) DisCenter = 500;
//	    if (DisLeft == 0) DisLeft = 500;
//	    if (DisRight == 0) DisRight = 500;
//
//	    if (DisCenter < 30) // 정면 장애물
//	    {
//	        Car_Stop();
//	        HAL_Delay(100); // 잠시 멈춤
//	        if (DisLeft > DisRight) {
//	            Car_Move(CAR_LEFT, SPD_50); // 좀 더 빠른 속도로 회전
//	        } else {
//	            Car_Move(CAR_RIGHT, SPD_50);
//	        }
//	        HAL_Delay(300); // 회전할 시간 확보
//	    }
//	    else if (DisLeft < 20) { // 왼쪽이 가까우면 오른쪽으로
//	        Car_Move(CAR_RIGHT, SPD_30);
//	    }
//	    else if (DisRight < 20) { // 오른쪽이 가까우면 왼쪽으로
//	        Car_Move(CAR_LEFT, SPD_30);
//	    }
//	    else {
//	        Car_Move(CAR_FRONT, SPD_30);
//	    }



		SHOW_UART2();



}



static bool st_auto = 0;
static bool st_manual = 1;


void ST_FLAG(uint8_t cmd)
{
	if(cmd == 'A')
	{
		st_auto = 1;
		st_manual = 0;
		auto_st = AUTO_STATE_SCAN;
	}
	if(cmd == 'P')
	{
		Car_Stop();
		st_auto = 0;
		st_manual = 1;
		auto_st = AUTO_STATE_SCAN;

	}
}




void ST_MACHINE() {
	if (rxFlag)
	{
		HAL_UART_Transmit(&huart1, (uint8_t*) &rxCmd, 1, 10);
		ST_FLAG(rxCmd);

		if (st_manual == 1)
		{
			DC_CONTROL_MANUAL(rxCmd);
		}
		rxFlag = 0;
	}
	if (st_auto == 1)
	{
		DC_CONTROL_AUTO();
	}

}
