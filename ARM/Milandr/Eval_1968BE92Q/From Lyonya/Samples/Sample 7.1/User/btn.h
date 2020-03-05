/*******************************************************************************
 * @File: btn.h
 * @Author: Milandr, L.
 * @Project: Sample 2.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: ��������� �������� � ������������ ������
 *******************************************************************************/

#ifndef BTN_H
#define BTN_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� �������
void BTN_Init(void);                          // ������������� ������ ��� ������ � ��������
void Thread_DownButtonCheck(void *argument);  // ����� ������ ������ DOWN
void Thread_UpButtonCheck(void *argument);    // ����� ������ ������ UP

#endif
