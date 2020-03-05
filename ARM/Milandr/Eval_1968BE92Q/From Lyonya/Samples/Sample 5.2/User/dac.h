/*******************************************************************************
 * @File: dac.h
 * @Author: Milandr, L.
 * @Project: Sample 5.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: Управление цифро-аналоговым преобразователем
 *******************************************************************************/

#ifndef DAC_H
#define DAC_H

// Подключение стандартных библиотек
#include "link.h"

// Число Пи
#define PI 3.1415926535F

// Опорное напряжение ЦАП (в вольтах)
#define DAC_REF 3.290F

// Максимальное значение кода ЦАП
#define DAC_MAX 4095

// Макрос для перевода напряжения (в вольтах) в код ЦАП
#define VLT_TO_DGT(V) (uint16_t)(((V) / DAC_REF) * DAC_MAX)

// Количество отсчетов на период сигнала
#define SIGNAL_SAMPLE_SIZE 128

// Характеристики сигнала
#define SIGNAL_CONSTANT    1.00F   // Постоянная составляющая сигнала (в вольтах)
#define SIGNAL_AMPLITUDE   0.75F   // Амплитуда сигнала (в вольтах)
#define SIGNAL_FREQUENCY   2000    // Частота сигнала (в Гц)

// Массив отсчетов сигнала
extern volatile uint16_t signal[SIGNAL_SAMPLE_SIZE];

// Прототипы функций
void DAC_Reset(void);                           // Деинициализация цифро-аналогового преобразователя
void DAC_PortCfg(void);                         // Конфигурация линии в качестве выхода ЦАП
void DAC_Init(void);                            // Инициализация цифро-аналогового преобразователя
void GenerateSinus(volatile uint16_t *buffer);  // Генерация синусоидального сигнала
void GenerateSaw(volatile uint16_t *buffer);    // Генерация пилообразного сигнала
void DAC_PrintInfo(void);                       // Отображение параметров сигнала на дисплей

#endif
