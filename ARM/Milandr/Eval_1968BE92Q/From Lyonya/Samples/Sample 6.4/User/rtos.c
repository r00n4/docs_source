/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 6.4
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Идентификатор объекта событий
osEventFlagsId_t EventId_ADC;

// Идентификаторы виртуальных таймеров
osTimerId_t TimerId_MoveDoggie;
osTimerId_t TimerId_TimeCounter;
osTimerId_t TimerId_GameOver;

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потока аналого-цифрового преобразования
  osThreadNew(Thread_ADC, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 96});

  // Создание потока инициализации игры
  osThreadNew(Thread_DoggieInit, NULL, &(osThreadAttr_t){.priority = osPriorityHigh, .stack_size = 296});

  // Создание объекта событий
  EventId_ADC = osEventFlagsNew(NULL);

  // Создание виртуальных таймеров
  TimerId_MoveDoggie  = osTimerNew(TimerCallback_MoveDoggie,  osTimerPeriodic, NULL, NULL);
  TimerId_TimeCounter = osTimerNew(TimerCallback_TimeCounter, osTimerPeriodic, NULL, NULL);
  TimerId_GameOver    = osTimerNew(TimerCallback_GameOver,    osTimerOnce,     NULL, NULL);

  // Запуск ядра ОСРВ
  osKernelStart();
}
