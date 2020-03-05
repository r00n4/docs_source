/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 6.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ������� �������
osEventFlagsId_t EventId_ADC;

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������ �������-��������� ��������������
  osThreadNew(Thread_ADC, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 224});

  // �������� ������ ������ ������ ���������
  osThreadNew(Thread_JoystickButtonCheck, NULL, &(osThreadAttr_t){.priority = osPriorityLow, .stack_size = 160});

  // �������� ������� �������
  EventId_ADC = osEventFlagsNew(NULL);

  // ������ ���� ����
  osKernelStart();
}
