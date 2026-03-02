/*
 * ultrasonic.c
 *
 *  Created on: Feb 28, 2026
 *      Author: lenovo
 */


#include "ultrasonic.h"
#include "gpio.h"
#include "tim.h"
#include "delay.h"


/* ====== TRIG 핀 매핑 ====== */
#define TRIG_PORT_LEFT    GPIOC
#define TRIG_PIN_LEFT     GPIO_PIN_8

#define TRIG_PORT_CENTER  GPIOC
#define TRIG_PIN_CENTER   GPIO_PIN_6

#define TRIG_PORT_RIGHT   GPIOC
#define TRIG_PIN_RIGHT    GPIO_PIN_5


/* ====== 내부 채널 테이블 ====== */
static ultrasonic_ch_t s_us[US_COUNT];
static uint32_t prev_tick = 0;
static int st = 0;


static void trig_pulse(GPIO_TypeDef *port, uint16_t pin)
{
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
  delay_us(1);
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
  delay_us(10);
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}


/* ====== 캡처 처리 ====== */


static void process_capture(ultrasonic_ch_t *ch, TIM_HandleTypeDef *htim)
{
  uint32_t captured_val = HAL_TIM_ReadCapturedValue(htim, ch->tim_channel);

  if (ch->capture_flag == 0) // Rising Edge
  {
    ch->ic_v1 = captured_val;
    ch->capture_flag = 1;
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, ch->tim_channel, TIM_INPUTCHANNELPOLARITY_FALLING);
  }
  else // Falling Edge
  {
    ch->ic_v2 = captured_val;

    // 시간차 계산 (오버플로우 고려)
    if (ch->ic_v2 >= ch->ic_v1)
      ch->echo_time_us = ch->ic_v2 - ch->ic_v1;
    else
      ch->echo_time_us = (65535 - ch->ic_v1) + ch->ic_v2;

    ch->distance_cm = (uint8_t)(ch->echo_time_us / 58);
    ch->capture_flag = 0;
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, ch->tim_channel, TIM_INPUTCHANNELPOLARITY_RISING);
  }
}




//static void process_capture(ultrasonic_ch_t *ch, TIM_HandleTypeDef *htim)
//{
//  if (ch->capture_flag == 0)
//  {
//    /* Rising edge */
//	  ch->ic_v1 = HAL_TIM_ReadCapturedValue(htim, ch->tim_channel); // 시작 시간 저장
//    __HAL_TIM_SET_COUNTER(htim, 0);   // 여기서 카운터 0
//    ch->capture_flag = 1;
//
//    __HAL_TIM_SET_CAPTUREPOLARITY(htim,
//                                  ch->tim_channel,
//                                  TIM_INPUTCHANNELPOLARITY_FALLING);
//  }
//  else
//  {
//    /* Falling edge */
//	  ch->ic_v2 = HAL_TIM_ReadCapturedValue(htim, ch->tim_channel); // 종료 시간 저장
//    uint16_t captured = HAL_TIM_ReadCapturedValue(htim, ch->tim_channel);
//
//    if (ch->ic_v2 > ch->ic_v1)
//      ch->echo_time_us = ch->ic_v2 - ch->ic_v1;
//    else
//      ch->echo_time_us = (0xFFFF - ch->ic_v1) + ch->ic_v2; // 오버플로우 처리
//
//    ch->echo_time_us = captured;          // counter=0부터 떨어질 때까지 시간
//    ch->distance_cm  = (uint8_t)(captured / 58u);
//
//    ch->capture_flag = 0;
//
//    __HAL_TIM_SET_CAPTUREPOLARITY(htim,
//                                  ch->tim_channel,
//                                  TIM_INPUTCHANNELPOLARITY_RISING);
//  }
//}


//static void enable_cc_it(ultrasonic_ch_t *ch)
//{
//  __HAL_TIM_ENABLE_IT(ch->htim, ch->tim_it_cc);
//}
//
//static void disable_cc_it(ultrasonic_ch_t *ch)
//{
//  __HAL_TIM_DISABLE_IT(ch->htim, ch->tim_it_cc);
//}
//
///* 캡처 2번으로 echo width(us) 계산 -> cm 변환 */
//static void process_capture(ultrasonic_ch_t *ch, TIM_HandleTypeDef *htim)
//{
//  if (ch->capture_flag == 0)
//  {
//    ch->ic_v1 = (uint16_t)HAL_TIM_ReadCapturedValue(htim, ch->tim_channel);
//    ch->capture_flag = 1;
//    __HAL_TIM_SET_CAPTUREPOLARITY(htim, ch->tim_channel, TIM_INPUTCHANNELPOLARITY_FALLING);
//  }
//  else
//  {
//    ch->ic_v2 = (uint16_t)HAL_TIM_ReadCapturedValue(htim, ch->tim_channel);
//    __HAL_TIM_SET_COUNTER(htim, 0);
//
//    if (ch->ic_v2 >= ch->ic_v1)
//      ch->echo_time_us = (uint16_t)(ch->ic_v2 - ch->ic_v1);
//    else
//      ch->echo_time_us = (uint16_t)((0xFFFFu - ch->ic_v1) + ch->ic_v2);
//
//    /* us/58 = cm */
//    ch->distance_cm = (uint8_t)(ch->echo_time_us / 58u);
//
//    ch->capture_flag = 0;
//    __HAL_TIM_SET_CAPTUREPOLARITY(htim, ch->tim_channel, TIM_INPUTCHANNELPOLARITY_RISING);
//
//    /* 측정 끝났으면 해당 CC 인터럽트 꺼서 튐 방지 */
//    disable_cc_it(ch);
//  }
//}


void Ultrasonic_Init(void)
{
  s_us[US_LEFT] = (ultrasonic_ch_t){
    .trig_port = TRIG_PORT_LEFT,
    .trig_pin  = TRIG_PIN_LEFT,
    .htim      = &htim3,
    .tim_channel = TIM_CHANNEL_4,
    .tim_it_cc   = TIM_IT_CC4,
    .capture_flag = 0
  };

  s_us[US_CENTER] = (ultrasonic_ch_t){
    .trig_port = TRIG_PORT_CENTER,
    .trig_pin  = TRIG_PIN_CENTER,
    .htim      = &htim4,
    .tim_channel = TIM_CHANNEL_3,
    .tim_it_cc   = TIM_IT_CC3,
    .capture_flag = 0
  };

  s_us[US_RIGHT] = (ultrasonic_ch_t){
    .trig_port = TRIG_PORT_RIGHT,
    .trig_pin  = TRIG_PIN_RIGHT,
    .htim      = &htim4,
    .tim_channel = TIM_CHANNEL_4,
    .tim_it_cc   = TIM_IT_CC4,
    .capture_flag = 0
  };

  /* Input Capture Start */
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_4);

  /* Rising edge 시작 */
  __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
  __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
  __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
}




//void Ultrasonic_Init(void)
//{
//  /* 채널 매핑 */
//  s_us[US_LEFT] = (ultrasonic_ch_t){
//    .trig_port = TRIG_PORT_LEFT,
//    .trig_pin  = TRIG_PIN_LEFT,
//    .htim      = &htim3,
//    .tim_channel = TIM_CHANNEL_4,
//    .tim_it_cc   = TIM_IT_CC4,
//    .ic_v1 = 0, .ic_v2 = 0, .echo_time_us = 0, .distance_cm = 0, .capture_flag = 0
//  };
//
//  s_us[US_CENTER] = (ultrasonic_ch_t){
//    .trig_port = TRIG_PORT_CENTER,
//    .trig_pin  = TRIG_PIN_CENTER,
//    .htim      = &htim4,
//    .tim_channel = TIM_CHANNEL_3,
//    .tim_it_cc   = TIM_IT_CC3,
//    .ic_v1 = 0, .ic_v2 = 0, .echo_time_us = 0, .distance_cm = 0, .capture_flag = 0
//  };
//
//  s_us[US_RIGHT] = (ultrasonic_ch_t){
//    .trig_port = TRIG_PORT_RIGHT,
//    .trig_pin  = TRIG_PIN_RIGHT,
//    .htim      = &htim4,
//    .tim_channel = TIM_CHANNEL_4,
//    .tim_it_cc   = TIM_IT_CC4,
//    .ic_v1 = 0, .ic_v2 = 0, .echo_time_us = 0, .distance_cm = 0, .capture_flag = 0
//  };
//
//  /* IC Start IT */
//  HAL_TIM_IC_Start_IT(s_us[US_LEFT].htim,   s_us[US_LEFT].tim_channel);
//  HAL_TIM_IC_Start_IT(s_us[US_CENTER].htim, s_us[US_CENTER].tim_channel);
//  HAL_TIM_IC_Start_IT(s_us[US_RIGHT].htim,  s_us[US_RIGHT].tim_channel);
//
//  /* 시작은 Rising으로 */
//  __HAL_TIM_SET_CAPTUREPOLARITY(s_us[US_LEFT].htim,   s_us[US_LEFT].tim_channel,   TIM_INPUTCHANNELPOLARITY_RISING);
//  __HAL_TIM_SET_CAPTUREPOLARITY(s_us[US_CENTER].htim, s_us[US_CENTER].tim_channel, TIM_INPUTCHANNELPOLARITY_RISING);
//  __HAL_TIM_SET_CAPTUREPOLARITY(s_us[US_RIGHT].htim,  s_us[US_RIGHT].tim_channel,  TIM_INPUTCHANNELPOLARITY_RISING);
//
//  /* 측정 전까지 CC 인터럽트는 꺼둬도 됨(Trigger 때 켬) */
//  disable_cc_it(&s_us[US_LEFT]);
//  disable_cc_it(&s_us[US_CENTER]);
//  disable_cc_it(&s_us[US_RIGHT]);
//}


void Ultrasonic_TriggerAll(void)
{
	uint32_t current_tick = HAL_GetTick();

	if (current_tick - prev_tick < 30) return;

	switch (st) {
		case 0:
		    Ultrasonic_TriggerOne(US_LEFT);
		    st = 1;
			break;
		case 1:
		    Ultrasonic_TriggerOne(US_CENTER);
		    st = 2;
			break;
		case 2:
		    Ultrasonic_TriggerOne(US_RIGHT);
		    st = 0;
			break;
		default:
			break;
	}

	prev_tick = current_tick;

//    HAL_Delay(30);
//    Ultrasonic_TriggerOne(US_CENTER);
//    HAL_Delay(30);
//    Ultrasonic_TriggerOne(US_RIGHT);
//    HAL_Delay(30);
}



//void Ultrasonic_TriggerAll(void)
//{
//  /* LEFT */
//  trig_pulse(s_us[US_LEFT].trig_port, s_us[US_LEFT].trig_pin);
//  enable_cc_it(&s_us[US_LEFT]);
//
//  /* CENTER */
//  trig_pulse(s_us[US_CENTER].trig_port, s_us[US_CENTER].trig_pin);
//  enable_cc_it(&s_us[US_CENTER]);
//
//  /* RIGHT */
//  trig_pulse(s_us[US_RIGHT].trig_port, s_us[US_RIGHT].trig_pin);
//  enable_cc_it(&s_us[US_RIGHT]);
//}


void Ultrasonic_TriggerOne(ultrasonic_id_t id)
{
  if (id >= US_COUNT) return;
  trig_pulse(s_us[id].trig_port, s_us[id].trig_pin);
}



//void Ultrasonic_TriggerOne(ultrasonic_id_t id)
//{
//  if (id >= US_COUNT) return;
//  trig_pulse(s_us[id].trig_port, s_us[id].trig_pin);
//  enable_cc_it(&s_us[id]);
//}

uint8_t Ultrasonic_GetDistanceCm(ultrasonic_id_t id)
{
  if (id >= US_COUNT) return 0;
  return s_us[id].distance_cm;
}

void Ultrasonic_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3 &&
      htim->Channel  == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    process_capture(&s_us[US_LEFT], htim);
    return;
  }

  if (htim->Instance == TIM4 &&
      htim->Channel  == HAL_TIM_ACTIVE_CHANNEL_3)
  {
    process_capture(&s_us[US_CENTER], htim);
    return;
  }

  if (htim->Instance == TIM4 &&
      htim->Channel  == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    process_capture(&s_us[US_RIGHT], htim);
    return;
  }
}

