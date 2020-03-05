/*******************************************************************************
 * @File: btn.h
 * @Author: Milandr, L.
 * @Project: Sample 6.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: ������������ ����� �����-������ ��� ������ � ��������
 *******************************************************************************/

#ifndef BTN_H
#define BTN_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� �������
void BTN_Init(void);                              // ������������� ����� ��� ������ � �������
void Thread_JoystickButtonCheck(void *argument);  // ����� ������ ������ ���������

#endif
