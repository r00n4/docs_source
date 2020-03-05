/*******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: Sample 6.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 24.03.2017
 * @Purpose: Управление аналого-цифровым преобразователем
 *******************************************************************************/

#ifndef ADC_H
#define ADC_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Калибровочные константы напряжения
#define CALIBR_CONST_V 3.290F
#define CALIBR_CONST_D 0xFFF

// Макрос для расчета напряжения
#define VOLTAGE(D) CALIBR_CONST_V * (D) / CALIBR_CONST_D

// Прототипы функций
void ADC_Reset(void);             // Деинициализация аналого-цифровых преобразователей
void ADC_PortCfg(void);           // Конфигурация порта для работы АЦП
void ADC_Init(void);              // Инициализация АЦП
void Thread_ADC(void *argument);  // Поток аналого-цифровых преобразований

#endif
