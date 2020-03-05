/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 7.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������� ������ ������
  osThreadNew(Thread_DownButtonCheck, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 232});
  osThreadNew(Thread_UpButtonCheck,   NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 232});

  // ������ ���� ����
  osKernelStart();
}
