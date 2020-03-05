/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 12.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.08.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "rtos.h"

// ������������� ������� �������
osMessageQueueId_t MsgQueId_CAN;

// ������������� ������������ ������� ��������� �������
void RTOS_Init(void)
{
  // ������������� ���� ����
  osKernelInitialize();

  // �������� ������ ����������� CAN
  osThreadNew(Thread_Transmitter, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 80});

  // �������� ������ ����������� CAN
  osThreadNew(Thread_Receiver, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 240});

  // �������� ������� �������
  MsgQueId_CAN = osMessageQueueNew(MSG_AMOUNT, MSG_SIZE, NULL);

  // ������ ���� ����
  osKernelStart();
}
