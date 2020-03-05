/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 6.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 15.05.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Идентификатор очереди сообщений
osMessageQueueId_t MsgQueId_ADC;

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потока аналого-цифрового преобразования
  osThreadNew(Thread_ADC, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 592});

  // Создание очереди сообщений
  MsgQueId_ADC = osMessageQueueNew(MSG_AMOUNT, MSG_SIZE, NULL);

  // Запуск ядра ОСРВ
  osKernelStart();
}
