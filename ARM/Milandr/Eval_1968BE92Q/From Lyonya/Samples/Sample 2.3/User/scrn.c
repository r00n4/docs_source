/*******************************************************************************
 * @File: scrn.c
 * @Author: Milandr, L.
 * @Project: Sample 2.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 05.04.2017
 * @Purpose: ����������� ���������� �� �������������������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "scrn.h"

// ������� ��������� �������
uint8_t current_state = RED;

// ����������� ����
void LCD_MenuInit(void)
{
  LCD_PutString(" # RED",    3);
  LCD_PutString("   YELLOW", 4);
  LCD_PutString("   GREEN",  5);
  LCD_PutString("   BLUE",   6);
}

// ���������� ��������� �������
void LCD_ImplementState(uint8_t state)
{
  uint8_t i;

  // ��������� ��������
  osMutexAcquire(MutexId_Display, osWaitForever);

  // �������� ���� �������� � �������
  for (i = 0; i < 4; i++)
    LCD_PutSymbol(' ', 1, (3 + i));

  // ��������� ��������� �� �������
  switch (state)
  {
    // ��������� "RED"
    case RED:
      // ����������� �������
      LCD_PutSymbol('#', 1, 3);
      break;

    // ��������� "YELLOW"
    case YELLOW:
      // ����������� �������
      LCD_PutSymbol('#', 1, 4);
      break;

    // ��������� "GREEN"
    case GREEN:
      // ����������� �������
      LCD_PutSymbol('#', 1, 5);
      break;

    // ��������� "BLUE"
    case BLUE:
      // ����������� �������
      LCD_PutSymbol('#', 1, 6);
      break;

    // �������������� ���������
    default:
      // ������� � ��������� "RED"
      state = RED;
      break;
  }

  // ������������ ��������
  osMutexRelease(MutexId_Display);
}
