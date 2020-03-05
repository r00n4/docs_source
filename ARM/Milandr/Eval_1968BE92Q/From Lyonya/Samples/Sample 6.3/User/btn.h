/*******************************************************************************
 * @File: btn.h
 * @Author: Milandr, L.
 * @Project: Sample 6.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: Конфигурация линий ввода-вывода для работы с кнопками
 *******************************************************************************/

#ifndef BTN_H
#define BTN_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Прототипы функций
void BTN_Init(void);                              // Инициализация порта для работы с кнопкой
void Thread_JoystickButtonCheck(void *argument);  // Поток опроса кнопки джойстика

#endif
