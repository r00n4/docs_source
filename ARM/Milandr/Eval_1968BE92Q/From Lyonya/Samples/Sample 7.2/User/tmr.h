/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 7.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 10.12.2018
 * @Purpose: Конфигурация аппаратных таймеров
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Делитель тактовой частоты таймера TIMER1
#define TIMER1_PRESCALER 1

// Частота широтно-импульсной модуляции (в Гц)
#define PULSE_FREQUENCY 100000

// Макрос для расчета периода импульсов
#define PULSE_PERIOD(F) (uint16_t)(SystemCoreClock / ((uint32_t)(F) * TIMER1_PRESCALER) - 1)

// Макрос для расчета коэффициента заполнения

// #define DUTY_CYCLE(D) (uint16_t)(PULSE_PERIOD * (D) / 0xFFF)

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // Деинициализация таймера
void TIMER_PortCfg(void);                         // Конфигурация порта для работы с таймером
void TIMER_Init(void);                            // Инициализация таймера (TIMER1)

#endif
