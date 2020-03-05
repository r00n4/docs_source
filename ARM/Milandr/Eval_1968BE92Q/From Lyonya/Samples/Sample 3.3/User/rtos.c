/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 3.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 12.07.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// �������������� ��������
osTimerId_t TimerId_Counter;
osTimerId_t TimerId_Warning;

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������ ������ �� ���������� ��������
  osThreadNew(Thread_Watchdog, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 400});

  // �������� ������������ ������� ��� �������� ������� ������ �������
  TimerId_Counter = osTimerNew(TimerCallback_Counter, osTimerPeriodic, NULL, NULL);

  // �������� ������������ ������� ��� ����������� ��������������
  TimerId_Warning = osTimerNew(TimerCallback_Warning, osTimerOnce, NULL, NULL);

  // ������ ���� ����
  osKernelStart();
}
