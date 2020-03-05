/*******************************************************************************
 * @File: iwdg.c
 * @Author: Milandr, L.
 * @Project: Sample 3.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 12.07.2017
 * @Purpose: Конфигурация сторожевого таймера
 *******************************************************************************/

#ifndef IWDG_H
#define IWDG_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Прототипы функций
void IWDG_Init(void);                        // Инициализация сторожевого таймера
void Thread_Watchdog(void *argument);        // Поток работы со сторожевым таймером
void TimerCallback_Counter(void *argument);  // Процедура отсчета времени, вызываемая таймером
void TimerCallback_Warning(void *argument);  // Функция предупреждения, вызываемая таймером

#endif
