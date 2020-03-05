/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 3.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 13.03.2019
 * @Purpose: Конфигурация аппаратных таймеров/счетчиков
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Делитель тактовой частоты TIMER1
#define PRESCALER 620

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // Деинициализация аппаратного таймера
void TIMER_Init(void);                            // Инициализация аппаратного таймера

#endif
