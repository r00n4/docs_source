/*******************************************************************************
 * @File: scrn.h
 * @Author: Milandr, L.
 * @Project: Sample 2.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 05.04.2017
 * @Purpose: Отображение информации на жидкокристаллический дисплей
 *******************************************************************************/

#ifndef SCRN_H
#define SCRN_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Прототипы функций
void Thread_Ticker(void *argument);  // Поток управления бегущей строкой

#endif
