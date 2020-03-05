/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 7.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 10.12.2018
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

  // �������� ������� �������
  EventId_ADC = osEventFlagsNew(NULL);

  // ������ ���� ����
  osKernelStart();
}
