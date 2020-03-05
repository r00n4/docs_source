/*******************************************************************************
 * @File: led.h
 * @Author: Milandr, L.
 * @Project: Sample 2.1
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
enum SYSTEM_STATES
{
  RED = 1,
  YELLOW,
  GREEN,
  BLUE
};

// ��������� �������
void LED_Init(void);                     // ������������� ����� ��� ������ �� ������������
void LED_ImplementState(uint8_t state);  // ���������� ��������� �������

#endif
