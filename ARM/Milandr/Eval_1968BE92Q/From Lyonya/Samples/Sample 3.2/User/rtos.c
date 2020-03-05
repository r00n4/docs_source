/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 3.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 15.03.2019
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потока измерения длительности задержки
  osThreadNew(Thread_TimeCounter, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 576});

  // Создание потока опроса кнопки
  osThreadNew(Thread_ButtonCheck, NULL, &(osThreadAttr_t){.priority = osPriorityHigh,   .stack_size = 576});

  // Запуск ядра ОСРВ
  osKernelStart();
}
