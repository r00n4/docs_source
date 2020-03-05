/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 5.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ������� �������
osEventFlagsId_t EventId_UART;

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������ �������� �������� � ���������� �� �� ������� ���������� �����
  osThreadNew(Thread_Typewriter, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 112});

  // �������� ������� �������
  EventId_UART = osEventFlagsNew(NULL);

  // ������ ���� ����
  osKernelStart();
}
