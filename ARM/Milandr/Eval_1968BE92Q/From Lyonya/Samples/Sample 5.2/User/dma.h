/*******************************************************************************
 * @File: dma.h
 * @Author: Milandr, L.
 * @Project: Sample 5.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: Конфигурация прямого доступа к памяти
 *******************************************************************************/

#ifndef DMA_H
#define DMA_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Прототипы функций
void DMA_Reset(void);  // Деинициализация контроллера DMA
void DMA_Init(void);   // Инициализация контроллера DMA

#endif
