/*******************************************************************************
 * @File: btn.h
 * @Author: Milandr, L.
 * @Project: Sample 3.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 15.03.2019
 * @Purpose: ������������ ����� �����-������ ��� ������ � ��������
 *******************************************************************************/

#ifndef BTN_H
#define BTN_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� �������
void BTN_Init(void);                      // ������������� ����� ��� ������ � �������
void Thread_ButtonCheck(void *argument);  // ����� ������ ������

#endif
