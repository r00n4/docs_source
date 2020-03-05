/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 5.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.03.2017
 * @Purpose: Конфигурация аппаратных таймеров
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Макрос для рассчета периода дискретизации по заданной частоте сигнала
#define SAMPLING_PERIOD(F) (uint16_t)(SystemCoreClock / (uint32_t)((F) * SIGNAL_SAMPLE_SIZE) - 1)

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // Деинициализация аппаратного таймера
void TIMER_Init(void);                            // Инициализация аппаратного таймера TIMER1

#endif
