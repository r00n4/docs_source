/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 2.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 11.05.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Поток мигания красным и желтым светодиодами
  osThreadNew(Thread_BlinkRY, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 80});

  // Поток мигания зеленым и синим светодиодами
  osThreadNew(Thread_BlinkGB, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 80});

  // Создание потока управления бегущей строкой
  osThreadNew(Thread_Ticker, NULL, &(osThreadAttr_t){.priority = osPriorityHigh,    .stack_size = 256});

  // Запуск ядра ОСРВ
  osKernelStart();
}
