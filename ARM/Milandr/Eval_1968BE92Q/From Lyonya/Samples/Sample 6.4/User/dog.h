/*******************************************************************************
 * @File: dog.h
 * @Author: Milandr, L.
 * @Project: Sample 6.4
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: Реализация алгоритма игры
 *******************************************************************************/

#ifndef DOG_H
#define DOG_H

// Подключение стандартной библиотеки языка Си
#include "stdlib.h"

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Время партии (в миллисекундах)
#define DOGGIE_GAME_TIME 60001

// Интервал между передвижениями "собачки" (в миллисекундах)
#define DOGGIE_MOVE_TIME 1807

// Границы игрового поля
#define FIELD_X_MIN 4
#define FIELD_X_MAX 16
#define FIELD_Y_MIN 2
#define FIELD_Y_MAX 5

// Прототипы функций
void DOG_MoveCursor(uint16_t digitX, uint16_t digitY);  // Передвижение указателя на дисплее
void Thread_DoggieInit(void *argument);                 // Поток инициализации игры
void TimerCallback_MoveDoggie(void *argument);          // Процедура передвижения "собачки", вызываемая таймером
void TimerCallback_TimeCounter(void *argument);         // Процедура отсчета времени игры, вызываемая таймером
void TimerCallback_GameOver(void *argument);            // Процедура завершения игры, вызываемая таймером

#endif
