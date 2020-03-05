/*******************************************************************************
 * @File: rtos.h
 * @Author: Milandr, L.
 * @Project: Sample 3.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 12.07.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

#ifndef RTOS_H
#define RTOS_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Идентификаторы таймеров
extern osTimerId_t TimerId_Counter;
extern osTimerId_t TimerId_Warning;

// Прототипы функций
void RTOS_Init(void);  // Инициализация операционной системы реального времени

#endif
