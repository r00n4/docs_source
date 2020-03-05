/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 5.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Идентификатор объекта событий
osEventFlagsId_t EventId_UART;

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потока передачи символов с клавиатуры ПК на дисплей отладочной платы
  osThreadNew(Thread_Typewriter, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 112});

  // Создание объекта событий
  EventId_UART = osEventFlagsNew(NULL);

  // Запуск ядра ОСРВ
  osKernelStart();
}
