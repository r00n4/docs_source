/*******************************************************************************
 * @File: iwdg.c
 * @Author: Milandr, L.
 * @Project: Sample 3.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 12.07.2017
 * @Purpose: Конфигурация сторожевого таймера
 *******************************************************************************/

// Подключение заголовочного файла
#include "iwdg.h"

// Инициализация сторожевого таймера IWDG
void IWDG_Init(void)
{
  // Включение генератора LSI
  MDR_BKP->REG_0F |= (1 << BKP_REG_0F_LSI_ON_Pos);

  // Ожидание входа генератора LSI в рабочий режим
  while ((MDR_BKP->REG_0F & (1 << BKP_REG_0F_LSI_RDY_Pos)) == 0);

  // Подстройка тактовой частоты генератора LSI
  MDR_BKP->REG_0F &= ~BKP_REG_0F_LSI_TRIM_Msk;
  MDR_BKP->REG_0F |= (11 << BKP_REG_0F_LSI_TRIM_Pos);

  // Включение тактирования сторожевого таймера
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_IWDT_Pos);

  // Разрешение записи данных в регистры PR и RLR
  MDR_IWDG->KR = 0x5555;

  // Установка делителя частоты сторожевого таймера
  while ((MDR_IWDG->SR & IWDG_SR_PVU) != 0);  // Проверка флага обновления делителя частоты
  MDR_IWDG->PR = 4;                           // Задание делителя (40 КГц / 64)

  // Установка значения перезагрузки сторожевого таймера
  while ((MDR_IWDG->SR & IWDG_SR_RVU) != 0);  // Проверка флага обновления значения перезагрузки
  MDR_IWDG->RLR = 3125;                       // Задание значения перезагрузки

  // Запуск сторожевого таймера
  MDR_IWDG->KR = 0xCCCC;
}

// Поток работы со сторожевым таймером
void Thread_Watchdog(void *argument)
{
  // Начальная перезагрузка сторожевого таймера
  MDR_IWDG->KR = 0xAAAA;

  // Начальное отображение времени работы системы
  TimerCallback_Counter(NULL);

  // Запуск виртуального таймера для отсчета времени работы системы
  osTimerStart(TimerId_Counter, 1000);

  // Основной цикл
  while (1)
  {
    // Запуск таймера, выводящего сообщение с предупреждением
    osTimerStart(TimerId_Warning, 2000);

    // Проверка нажатия кнопки SEL
    while ((MDR_PORTC->RXTX & (1 << 2)) != 0)
      osDelay(50);

    // Перезагрузка сторожевого таймера
    MDR_IWDG->KR = 0xAAAA;

    // Остановка виртуального таймера, выводящего сообщение с предупреждением
    osTimerStop(TimerId_Warning);

    // Удаление сообщения с предупреждением
    LCD_ClearString(5);

    // Задержка перед началом следующей итерации
    osDelay(50);
  }
}

// Процедура отсчета времени, вызываемая таймером
void TimerCallback_Counter(void *argument)
{
  static uint32_t t;          // Счетчик времени
  char message[LCD_STR_LEN];  // Сообщение, отображаемое на дисплей

  // Отображение текущего времени работы системы на дисплей
  snprintf(message, LCD_STR_LEN, "   Работа... %03u", t);
  LCD_PutString(message, 3);

  // Увеличение счетчика времени
  t++;
}

// Процедура предупреждения, вызываемая таймером
void TimerCallback_Warning(void *argument)
{
  // Отображение сообщение с предупреждением
  LCD_PutString("     Внимание!", 5);
}
