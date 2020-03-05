/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 3.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 15.03.2019
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������ ��������� ������������ ��������
  osThreadNew(Thread_TimeCounter, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 576});

  // �������� ������ ������ ������
  osThreadNew(Thread_ButtonCheck, NULL, &(osThreadAttr_t){.priority = osPriorityHigh,   .stack_size = 576});

  // ������ ���� ����
  osKernelStart();
}
