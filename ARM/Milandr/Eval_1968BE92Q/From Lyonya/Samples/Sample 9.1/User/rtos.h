/*******************************************************************************
 * @File: rtos.h
 * @Author: Milandr, L.
 * @Project: Sample 5.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

#ifndef RTOS_H
#define RTOS_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Псевдоним флага приема данных
#define EVENT_DATA_RECEIVED 0x00000001

// Идентификатор объекта событий
extern osEventFlagsId_t EventId_UART;

// Прототипы функций
void RTOS_Init(void);  // Инициализация операционной системы реального времени

#endif
