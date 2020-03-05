/*******************************************************************************
 * @File: rtos.h
 * @Author: Milandr, L.
 * @Project: Sample 6.4
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

#ifndef RTOS_H
#define RTOS_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Псевдоним флага завершения аналого-цифрового преобразования
#define EVENT_ADC_EOC 0x00000001

// Идентификатор объекта событий
extern osEventFlagsId_t EventId_ADC;

// Идентификаторы виртуальных таймеров
extern osTimerId_t TimerId_MoveDoggie;
extern osTimerId_t TimerId_TimeCounter;
extern osTimerId_t TimerId_GameOver;

// Прототипы функций
void RTOS_Init(void);  // Инициализация операционной системы реального времени

#endif
