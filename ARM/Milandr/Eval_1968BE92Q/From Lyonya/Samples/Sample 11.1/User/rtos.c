/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 11.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 22.05.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{ 
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������ ����������� �������
  osThreadNew(Thread_DisplayTime, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 280});

  // �������� ������ ��������� �������
  osThreadNew(Thread_SetTime, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 80});

  // ������ ���� ����
  osKernelStart();
}
