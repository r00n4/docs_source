/*******************************************************************************
 * @File: dac.c
 * @Author: Milandr, L.
 * @Project: Sample 5.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: Управление цифро-аналоговым преобразователем
 *******************************************************************************/

// Подключение заголовочного файла
#include "dac.h"

// Деинициализация цифро-аналогового преобразователя
void DAC_Reset(void)
{
  MDR_DAC->DAC2_DATA = 0;
  MDR_DAC->CFG       = 0;
}

// Конфигурация линии в качестве выхода ЦАП
void DAC_PortCfg(void)
{
  // Включение тактирования порта E
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTE_Pos);

  // Конфигурация линии PE0 в качестве вывода ЦАП
  MDR_PORTE->OE     &= ~(1 << 0);
  MDR_PORTE->FUNC   &= ~(3 << 0);
  MDR_PORTE->ANALOG &= ~(1 << 0);   // Режим работы линий (аналоговый)
  MDR_PORTE->PULL   &= ~(1 << 0);   // Подтяжка к земле (отключена)
  MDR_PORTE->PULL   &= ~(1 << 16);  // Подтяжка к питанию (отключена)
  MDR_PORTE->PD     &= ~(1 << 0);
  MDR_PORTE->PD     &= ~(1 << 16);
  MDR_PORTE->PWR    &= ~(3 << 0);
  MDR_PORTE->GFEN   &= ~(1 << 0);
}

// Инициализация цифро-аналогового преобразователя
void DAC_Init(void)
{
  // Конфигурация линии в качестве выхода ЦАП
  DAC_PortCfg();

  // Включение тактирования ЦАП
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_DAC_Pos);

  // Деинициализация ЦАП
  DAC_Reset();

  // Конфигурация ЦАП
  MDR_DAC->CFG = (0 << DAC_CFG_SYNC_A_Pos)    //
               | (0 << DAC_CFG_M_REF1_Pos)    // Выбор источника опорного напряжения (внутренний)
               | (1 << DAC_CFG_ON_DAC1_Pos);  // Работа DAC2 (включен)

  static const float voltage = 1.20;  // Требуемое значения напряжения (в вольтах)
  char message[LCD_STR_LEN];          // Сообщение, отображаемое на дисплей

  // Запуск цифро-аналогового преобразования
  MDR_DAC->DAC2_DATA = VLT_TO_DGT(voltage);

  // Отображение напряжения на дисплей
  snprintf(message, LCD_STR_LEN, "    V = %.2f В", voltage);
  LCD_PutString(message, 3);

  // Отображение кода ЦАП на дисплей
  snprintf(message, LCD_STR_LEN, "    D = %d", VLT_TO_DGT(voltage));
  LCD_PutString(message, 5);
}
