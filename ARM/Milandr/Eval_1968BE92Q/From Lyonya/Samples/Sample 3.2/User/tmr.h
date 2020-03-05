/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 3.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 15.03.2019
 * @Purpose: Конфигурация аппаратных таймеров/счетчиков
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Предделитель тактовой частоты TIMER1
#define TIMER1_PRESCALER 36622

// Макрос для преобразования отсчетов таймера во время (в секундах)
#define CNT_TO_TIME(CNT) ((float)(CNT) * TIMER1_PRESCALER / SystemCoreClock)

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // Деинициализация аппаратного таймера
void TIMER_Init(void);                            // Инициализация аппаратного таймера
void Thread_TimeCounter(void *argument);          // Поток отображения времени

#endif
