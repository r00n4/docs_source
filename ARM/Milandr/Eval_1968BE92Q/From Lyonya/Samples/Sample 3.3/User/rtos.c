/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 3.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 12.07.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Идентификаторы таймеров
osTimerId_t TimerId_Counter;
osTimerId_t TimerId_Warning;

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потока работы со сторожевым таймером
  osThreadNew(Thread_Watchdog, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 400});

  // Создание виртуального таймера для подсчета времени работы системы
  TimerId_Counter = osTimerNew(TimerCallback_Counter, osTimerPeriodic, NULL, NULL);

  // Создание виртуального таймера для отображения предупреждения
  TimerId_Warning = osTimerNew(TimerCallback_Warning, osTimerOnce, NULL, NULL);

  // Запуск ядра ОСРВ
  osKernelStart();
}
