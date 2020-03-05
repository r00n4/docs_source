/*******************************************************************************
 * @File: rtos.h
 * @Author: Milandr, L.
 * @Project: Sample 5.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

#ifndef RTOS_H
#define RTOS_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� ����� ������ ������
#define EVENT_DATA_RECEIVED 0x00000001

// ������������� ������� �������
extern osEventFlagsId_t EventId_UART;

// ��������� �������
void RTOS_Init(void);  // ������������� ������������ ������� ��������� �������

#endif
