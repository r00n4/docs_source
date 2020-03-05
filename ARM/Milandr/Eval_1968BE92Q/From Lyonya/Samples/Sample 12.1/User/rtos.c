/*******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: Sample 12.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.08.2017
 * @Purpose: Управление операционной системой реального времени
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtos.h"

// Идентификатор объекта событий
osMessageQueueId_t MsgQueId_CAN;

// Инициализация операционной системы реального времени
void RTOS_Init(void)
{
  // Инициализация ядра ОСРВ
  osKernelInitialize();

  // Создание потока передатчика CAN
  osThreadNew(Thread_Transmitter, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 80});

  // Создание потока передатчика CAN
  osThreadNew(Thread_Receiver, NULL, &(osThreadAttr_t){.priority = osPriorityNormal, .stack_size = 240});

  // Создание объекта событий
  MsgQueId_CAN = osMessageQueueNew(MSG_AMOUNT, MSG_SIZE, NULL);

  // Запуск ядра ОСРВ
  osKernelStart();
}
