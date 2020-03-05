/*******************************************************************************
 * @File: scrn.h
 * @Author: Milandr, L.
 * @Project: Sample 2.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 05.04.2017
 * @Purpose: ����������� ���������� �� �������������������� �������
 *******************************************************************************/

#ifndef SCRN_H
#define SCRN_H

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

// ������� ��������� �������
extern uint8_t current_state;

// ��������� �������
void LCD_MenuInit(void);                 // ����������� ����
void LCD_ImplementState(uint8_t state);  // ���������� ��������� �������

#endif
