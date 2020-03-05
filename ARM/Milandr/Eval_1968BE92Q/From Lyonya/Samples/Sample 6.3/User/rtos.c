/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 6.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Идентификатор объекта событий
osEventFlagsId_t EventId_ADC;

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потока аналого-цифрового преобразования
  osThreadNew(Thread_ADC, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 224});

  // Создание потока опроса кнопки джойстика
  osThreadNew(Thread_JoystickButtonCheck, NULL, &(osThreadAttr_t){.priority = osPriorityLow, .stack_size = 160});

  // Создание объекта событий
  EventId_ADC = osEventFlagsNew(NULL);

  // Запуск ядра ОСРВ
  osKernelStart();
}
