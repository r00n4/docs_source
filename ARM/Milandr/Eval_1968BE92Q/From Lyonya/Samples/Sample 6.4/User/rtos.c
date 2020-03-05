/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 6.4
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ������� �������
osEventFlagsId_t EventId_ADC;

// �������������� ����������� ��������
osTimerId_t TimerId_MoveDoggie;
osTimerId_t TimerId_TimeCounter;
osTimerId_t TimerId_GameOver;

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������ �������-��������� ��������������
  osThreadNew(Thread_ADC, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 96});

  // �������� ������ ������������� ����
  osThreadNew(Thread_DoggieInit, NULL, &(osThreadAttr_t){.priority = osPriorityHigh, .stack_size = 296});

  // �������� ������� �������
  EventId_ADC = osEventFlagsNew(NULL);

  // �������� ����������� ��������
  TimerId_MoveDoggie  = osTimerNew(TimerCallback_MoveDoggie,  osTimerPeriodic, NULL, NULL);
  TimerId_TimeCounter = osTimerNew(TimerCallback_TimeCounter, osTimerPeriodic, NULL, NULL);
  TimerId_GameOver    = osTimerNew(TimerCallback_GameOver,    osTimerOnce,     NULL, NULL);

  // ������ ���� ����
  osKernelStart();
}
