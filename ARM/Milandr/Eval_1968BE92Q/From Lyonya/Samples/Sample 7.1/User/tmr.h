/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 7.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: Конфигурация аппаратных таймеров
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Делитель тактовой частоты таймера TIMER1
#define TIMER1_PRESCALER 10

// Частота широтно-импульсной модуляции (в Гц)
#define PULSE_FREQUENCY 10000

// Шаг изменения коэффициента заполнения
#define DUTY_CYCLE_DELTA 10

// Макрос для расчета периода импульсов
#define PULSE_PERIOD(F) (uint16_t)(SystemCoreClock / ((uint32_t)(F) * TIMER1_PRESCALER) - 1)

// Коэффициент заполнения импульсов
extern int8_t duty_cycle;

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // Деинициализация таймера
void TIMER_PortCfg(void);                         // Конфигурация порта для работы с таймером
void TIMER_Init(void);                            // Инициализация таймера (TIMER1)
void PWM_SetDutyCycle(uint8_t duty_cycle);        // Задание скважности импульсов

#endif
