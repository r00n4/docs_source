/*******************************************************************************
 * @File: rtos.h
 * @Author: Milandr, L.
 * @Project: Sample 3.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 12.07.2017
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

#ifndef RTOS_H
#define RTOS_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// �������������� ��������
extern osTimerId_t TimerId_Counter;
extern osTimerId_t TimerId_Warning;

// ��������� �������
void RTOS_Init(void);  // ������������� ������������ ������� ��������� �������

#endif
