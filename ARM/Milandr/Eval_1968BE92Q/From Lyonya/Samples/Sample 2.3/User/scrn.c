/*******************************************************************************
 * @File: scrn.c
 * @Author: Milandr, L.
 * @Project: Sample 2.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 05.04.2017
 * @Purpose: Отображение информации на жидкокристаллический дисплей
 *******************************************************************************/

// Подключение заголовочного файла
#include "scrn.h"

// Текущее состояние системы
uint8_t current_state = RED;

// Отображение меню
void LCD_MenuInit(void)
{
  LCD_PutString(" # RED",    3);
  LCD_PutString("   YELLOW", 4);
  LCD_PutString("   GREEN",  5);
  LCD_PutString("   BLUE",   6);
}

// Реализация состояний системы
void LCD_ImplementState(uint8_t state)
{
  uint8_t i;

  // Получение мьютекса
  osMutexAcquire(MutexId_Display, osWaitForever);

  // Стирание всех символов в столбце
  for (i = 0; i < 4; i++)
    LCD_PutSymbol(' ', 1, (3 + i));

  // Положения указателя на дисплее
  switch (state)
  {
    // Состояние "RED"
    case RED:
      // Отображение символа
      LCD_PutSymbol('#', 1, 3);
      break;

    // Состояние "YELLOW"
    case YELLOW:
      // Отображение символа
      LCD_PutSymbol('#', 1, 4);
      break;

    // Состояние "GREEN"
    case GREEN:
      // Отображение символа
      LCD_PutSymbol('#', 1, 5);
      break;

    // Состояние "BLUE"
    case BLUE:
      // Отображение символа
      LCD_PutSymbol('#', 1, 6);
      break;

    // Неопределенное состояние
    default:
      // Переход в состояние "RED"
      state = RED;
      break;
  }

  // Освобождение мьютекса
  osMutexRelease(MutexId_Display);
}
