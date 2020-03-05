/*******************************************************************************
 * @File: scrn.h
 * @Author: Milandr, L.
 * @Project: Sample 2.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 05.04.2017
 * @Purpose: Отображение информации на жидкокристаллический дисплей
 *******************************************************************************/

#ifndef SCRN_H
#define SCRN_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Состояния системы
enum SYSTEM_STATES
{
  RED = 1,
  YELLOW,
  GREEN,
  BLUE
};

// Текущее состояние системы
extern uint8_t current_state;

// Прототипы функций
void LCD_MenuInit(void);                 // Отображение меню
void LCD_ImplementState(uint8_t state);  // Реализация состояний системы

#endif
