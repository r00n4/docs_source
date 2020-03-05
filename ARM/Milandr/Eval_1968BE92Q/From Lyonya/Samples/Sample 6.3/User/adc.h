/*******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: Sample 6.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: Управление аналого-цифровым преобразователем
 *******************************************************************************/

#ifndef ADC_H
#define ADC_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Калибровочные константы для напряжения
#define ADC_SAMPLE_SIZE 64

// Буфер для хранения результатов преобразования
extern volatile uint32_t sample[ADC_SAMPLE_SIZE];

// Прототипы функций
void ADC_Reset(void);             // Деинициализация аналого-цифровых преобразователей
void ADC_PortCfg(void);           // Конфигурация порта для работы АЦП
void ADC_Init(void);              // Инициализация аналого-цифрового преобразователя
void Thread_ADC(void *argument);  // Поток аналого-цифровых преобразований

#endif
