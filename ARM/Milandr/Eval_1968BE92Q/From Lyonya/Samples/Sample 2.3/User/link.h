/*******************************************************************************
 * @File: link.h
 * @Author: Milandr, L.
 * @Project: Sample 2.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.10.2017
 * @Purpose: Подключение заголовочных файлов модулей проекта
 *******************************************************************************/

#ifndef LINK_H
#define LINK_H

// Подключение стандартных периферийных библиотек
#include "1986VE9x.h"

// Подключение библиотеки для работы с ОСРВ
#include <rtx_os.h>

// Подключение заголовочных файлов модулей проекта
#include "rtos.h"
#include "led.h"
#include "btn.h"
#include "scrn.h"
#include "lcd.h"

#endif
