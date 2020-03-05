/*******************************************************************************
 * @File: btn.h
 * @Author: Milandr, L.
 * @Project: Sample 2.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: Обработка сигналов с механических кнопок
 *******************************************************************************/

#ifndef BTN_H
#define BTN_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Прототипы функций
void BTN_Init(void);                          // Инициализация портов для работы с кнопками
void Thread_DownButtonCheck(void *argument);  // Поток опроса кнопки DOWN
void Thread_UpButtonCheck(void *argument);    // Поток опроса кнопки UP

#endif
