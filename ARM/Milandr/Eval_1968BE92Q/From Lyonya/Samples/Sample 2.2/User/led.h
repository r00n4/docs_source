/*******************************************************************************
 * @File: led.h
 * @Author: Milandr, L.
 * @Project: Sample 2.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: ������������ ����� �����-������ ��� ������ �� ������������
 *******************************************************************************/

#ifndef LED_H
#define LED_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� �������
void LED_Init(void);                  // ������������� ����� ��� ������ �� ������������
void Thread_BlinkRY(void *argument);  // ����� ������� ������� � ������ ������������
void Thread_BlinkGB(void *argument);  // ����� ������� ������� � ����� ������������

#endif
