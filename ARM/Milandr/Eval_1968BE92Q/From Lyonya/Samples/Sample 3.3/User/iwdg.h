/*******************************************************************************
 * @File: iwdg.c
 * @Author: Milandr, L.
 * @Project: Sample 3.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 12.07.2017
 * @Purpose: ������������ ����������� �������
 *******************************************************************************/

#ifndef IWDG_H
#define IWDG_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� �������
void IWDG_Init(void);                        // ������������� ����������� �������
void Thread_Watchdog(void *argument);        // ����� ������ �� ���������� ��������
void TimerCallback_Counter(void *argument);  // ��������� ������� �������, ���������� ��������
void TimerCallback_Warning(void *argument);  // ������� ��������������, ���������� ��������

#endif
