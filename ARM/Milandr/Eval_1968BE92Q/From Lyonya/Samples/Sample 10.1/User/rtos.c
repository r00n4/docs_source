/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 10.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 19.05.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потока опроса микросхемы BMP280
  osThreadNew(Thread_BMP280, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 592});

  // Запуск ядра ОСРВ
  osKernelStart();
}
