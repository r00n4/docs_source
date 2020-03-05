/*******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: Sample 7.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 10.12.2018
 * @Purpose: Управление аналого-цифровым преобразователем
 *******************************************************************************/

#ifndef ADC_H
#define ADC_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Размер выборки
#define ADC_SAMPLE_SIZE 128

// Максимальное значение аналого-цифрового преобразования
#define D_MAX 0xFFF

// Буфер для хранения результатов преобразования
extern volatile uint16_t sample[ADC_SAMPLE_SIZE];

// Прототипы функций
void ADC_Reset(void);                                  // Деинициализация аналого-цифровых преобразователей
void ADC_PortCfg(void);                                // Конфигурация порта для работы АЦП
void ADC_Init(void);                                   // Инициализация АЦП
uint32_t ADC_MedianFilter(volatile uint16_t *buffer);  // Медианная фильтрация
void Thread_ADC(void *argument);                       // Поток аналого-цифровых преобразований

#endif
