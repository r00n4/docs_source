/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 2.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 11.05.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // ����� ������� ������� � ������ ������������
  osThreadNew(Thread_BlinkRY, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 80});

  // ����� ������� ������� � ����� ������������
  osThreadNew(Thread_BlinkGB, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 80});

  // �������� ������ ���������� ������� �������
  osThreadNew(Thread_Ticker, NULL, &(osThreadAttr_t){.priority = osPriorityHigh,    .stack_size = 256});

  // ������ ���� ����
  osKernelStart();
}
