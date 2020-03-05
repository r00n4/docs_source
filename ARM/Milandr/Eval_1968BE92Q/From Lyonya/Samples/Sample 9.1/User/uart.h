/*******************************************************************************
 * @File: uart.h
 * @Author: Milandr, L.
 * @Project: Sample 9.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 02.03.2017
 * @Purpose: Работа с контроллером UART
 *******************************************************************************/

#ifndef UART_H
#define UART_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Требуемая скорость передачи данных
#define UART_SPEED 115200

// Прототипы функций
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx);  // Деинициализация контроллера UART
void UART_PortCfg(void);                       // Конфигурация порта для работы с контроллером UART (UART2)
void UART_Init(void);                          // Инициализация модуля UART (UART2)
void Thread_Typewriter(void *argument);        // Поток передачи символов с клавиатуры ПК на дисплей отладочной платы

#endif
