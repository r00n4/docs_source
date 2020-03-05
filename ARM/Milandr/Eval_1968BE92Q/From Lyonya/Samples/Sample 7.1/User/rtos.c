/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 7.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потоков опроса кнопок
  osThreadNew(Thread_DownButtonCheck, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 232});
  osThreadNew(Thread_UpButtonCheck,   NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 232});

  // Запуск ядра ОСРВ
  osKernelStart();
}
