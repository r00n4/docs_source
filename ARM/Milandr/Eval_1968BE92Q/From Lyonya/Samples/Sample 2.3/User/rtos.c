/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 2.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 11.05.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ��������
osMutexId_t MutexId_Display;

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������ �������� ������� ������ LEFT
  osThreadNew(Thread_DownButtonCheck, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 136});

  // �������� ������ �������� ������� ������ RIGHT
  osThreadNew(Thread_UpButtonCheck, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 136});

  // �������� ������ �������� ������� ������ SELECT
  osThreadNew(Thread_SelectButtonCheck, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 80});

  // �������� ��������
  MutexId_Display = osMutexNew(NULL);

  // ������ ���� ����
  osKernelStart();
}
