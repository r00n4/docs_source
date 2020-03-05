/*******************************************************************************
 * @File: rtc.h
 * @Author: Milandr, L.
 * @Project: Sample 11.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 08.02.2017
 * @Purpose: Работа с RTC DS1307
 *******************************************************************************/

#ifndef RTC_H
#define RTC_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Адрес микросхемы DS1337
#define RTC_I2C_ADDRESS 0x68

// Регистры микросхемы DS1307
#define RTC_REGISTER_SECONDS  0x00
#define RTC_REGISTER_MINUTES  0x01
#define RTC_REGISTER_HOURS    0x02
#define RTC_REGISTER_DAY      0x03
#define RTC_REGISTER_DATE     0x04
#define RTC_REGISTER_MONTH    0x05
#define RTC_REGISTER_YEAR     0x06
#define RTC_REGISTER_CONTROL  0x07

// Начальные время и дата
#define SECONDS 45  // 0-59
#define MINUTES 20  // 0-59
#define HOURS   10  // 0-23
#define DAY     5   // 1-7
#define DATE    29  // 1-31
#define MONTH   6   // 1-12
#define YEAR    18  // 0-99

// Прототипы функций
void RTC_SetTime(void);                               // Установка значений времени и даты
uint8_t RTC_ConvertData(uint8_t reg, uint8_t value);  // Преобразование данных в десятичный формат
void Thread_DisplayTime(void *argument);              // Поток отображения времени
void Thread_SetTime(void *argument);                  // Поток установки времени

#endif
