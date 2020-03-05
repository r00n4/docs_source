/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 11.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 22.05.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{ 
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потока отображения времени
  osThreadNew(Thread_DisplayTime, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 280});

  // Создание потока установки времени
  osThreadNew(Thread_SetTime, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 80});

  // Запуск ядра ОСРВ
  osKernelStart();
}
