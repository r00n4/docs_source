/*******************************************************************************
 * @File: rtc.c
 * @Author: Milandr, L.
 * @Project: Sample 11.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 08.02.2017
 * @Purpose: Работа с RTC DS1337
 *******************************************************************************/

// Подключение заголовочного файла
#include "rtc.h"

// Установка времени и даты
void RTC_SetTime (void)
{
  uint8_t i;

  // Массив начальных времени и даты
  uint8_t time[7] = {SECONDS, MINUTES, HOURS, DAY, DATE, MONTH, YEAR};

   // Конфигурация линий ввода-вывода для работы контроллера I2C
  I2C_PortCfg();

  // Цикл установки времени и даты
  for (i = 0; i < 7; i++)
  {
    // Преобразование данных в двоично-десятичный формат
    time[i] = (time[i] / 10) << 4 | (time[i] % 10);

    // Отправление данных
    I2C_TransmitData(RTC_I2C_ADDRESS, RTC_REGISTER_SECONDS + i, time[i]);
  }
}

// Преобразование данных в десятичный формат
uint8_t RTC_ConvertData(uint8_t register_address, uint8_t value)
{
  // Преобразование данных для каждого из используемых регистров
  switch (register_address)
  {
    // Регистр секунд
    case RTC_REGISTER_SECONDS:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x07);

    // Регистр минут
    case RTC_REGISTER_MINUTES:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x07);

    // Регистр часов
    case RTC_REGISTER_HOURS:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x03);

    // Регистр дня недели
    case RTC_REGISTER_DAY:
      return value & 0x07;

    // Регистр числа
    case RTC_REGISTER_DATE:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x03);

    // Регистр месяца
    case RTC_REGISTER_MONTH:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x01);

    // Регистр года
    case RTC_REGISTER_YEAR:
      return (value & 0x0F) + 10 * (value >> 4);

    // По умолчанию - без изменений
    default:
      return value;
  }
}

// Поток отображения времени
void Thread_DisplayTime(void *argument)
{
  uint8_t time[7];            // Массив для хранения даты и времени
  char message[LCD_STR_LEN];  // Сообщение, отображаемое на дисплей
  uint8_t error;              // Переменная для хранения кода ошибки

  // Основной цикл
  while (1)
  {
    // Конфигурация линий ввода-вывода для работы контроллера I2C
    I2C_PortCfg();

    // Чтение регистров времени и даты
    error = I2C_ReceiveDataBurst(RTC_I2C_ADDRESS, RTC_REGISTER_SECONDS, 7, time);

    // Если в процессе работы с шиной зафиксирована ошибка...
    if (error != 0)
    {
      // Запуск процедуры обработки ошибок
      I2C_ErrorHandler(error);
    }

    // В остальных случаях следует обработка принятых данных
    else
    {
      // Преобразование полученных данных в десятичный формат
      for (uint8_t i = 0; i < 7; i++)
        time[i] = RTC_ConvertData(RTC_REGISTER_SECONDS + i, time[i]);

      // Конфигурация линий ввода-вывода для работы дисплея
      LCD_PortCfg();

      // Удаление строки с информацией об ошибках
      LCD_ClearString(7);

      // Отображение времени на дисплей
      snprintf(message, LCD_STR_LEN, "      %02d:%02d:%02d", time[2], time[1], time[0]);
      LCD_PutString (message, 5);

      // Отображение даты на дисплей
      snprintf(message, LCD_STR_LEN, "     %02d.%02d.20%02d", time[4], time[5], time[6]);
      LCD_PutString(message, 3);
    }

    // Задержка перед началом следующей итерации
    osDelay(1000);
  }
}

// Поток установки времени
void Thread_SetTime(void *argument)
{
  // Основной цикл
  while (1)
  {
    // Проверка нажатия
    if ((MDR_PORTC->RXTX & (1 << 2)) == 0)
    {
      // Задержка для защиты от дребезга контактов
      osDelay(10);

      // Подтверждение нажатия
      if ((MDR_PORTC->RXTX & (1 << 2)) == 0)
      {
        // Установка заданного времени
        RTC_SetTime();

        // Ожидание отпускания
        while ((MDR_PORTC->RXTX & (1 << 2)) == 0)
          osDelay(25);
      }
    }

    // Задержка перед следующей проверкой
    osDelay(25);
  }
}
