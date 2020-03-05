/*******************************************************************************
 * @File: btn.h
 * @Author: Milandr, L.
 * @Project: Sample 3.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 15.03.2019
 * @Purpose: Конфигурация линий ввода-вывода для работы с кнопками
 *******************************************************************************/

#ifndef BTN_H
#define BTN_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Прототипы функций
void BTN_Init(void);                      // Инициализация порта для работы с кнопкой
void Thread_ButtonCheck(void *argument);  // Поток опроса кнопки

#endif
