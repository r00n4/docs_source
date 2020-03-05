/*******************************************************************************
 * @File: led.h
 * @Author: Milandr, L.
 * @Project: Sample 2.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: Конфигурация линий ввода-вывода для работы со светодиодами
 *******************************************************************************/

#ifndef LED_H
#define LED_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Прототипы функций
void LED_Init(void);                  // Инициализация порта для работы со светодиодами
void Thread_BlinkRY(void *argument);  // Поток мигания красным и желтым светодиодами
void Thread_BlinkGB(void *argument);  // Поток мигания зеленым и синим светодиодами

#endif
