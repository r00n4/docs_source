/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 6.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 15.05.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ������� ���������
osMessageQueueId_t MsgQueId_ADC;

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������ �������-��������� ��������������
  osThreadNew(Thread_ADC, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 592});

  // �������� ������� ���������
  MsgQueId_ADC = osMessageQueueNew(MSG_AMOUNT, MSG_SIZE, NULL);

  // ������ ���� ����
  osKernelStart();
}
