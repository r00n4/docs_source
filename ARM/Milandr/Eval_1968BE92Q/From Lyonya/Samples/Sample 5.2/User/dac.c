/*******************************************************************************
 * @File: dac.c
 * @Author: Milandr, L.
 * @Project: Sample 5.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: Управление цифро-аналоговым преобразователем
 *******************************************************************************/

// Подключение заголовочного файла
#include "dac.h"

// Массив отсчетов сигнала
volatile uint16_t signal[SIGNAL_SAMPLE_SIZE];

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
  MDR_DAC->CFG = (0 << DAC_CFG_M_REF1_Pos)    // Выбор источника опорного напряжения (внутренний)
               | (1 << DAC_CFG_ON_DAC1_Pos);  // Работа ЦАП (включен)
}

// Генерация синусоидального сигнала
void GenerateSinus(volatile uint16_t *buffer)
{
  float    omega;  // Период сигнала
  uint16_t i;

  // Деление тригонометрического периода на величину буфера
  omega = 2 * PI / SIGNAL_SAMPLE_SIZE;

  // Цикл формирования сигнала
  for (i = 0; i < SIGNAL_SAMPLE_SIZE; i++)
    buffer[i] = VLT_TO_DGT(SIGNAL_CONSTANT + SIGNAL_AMPLITUDE * sin(omega * i));
}

// Генерация пилообразного сигнала
void GenerateSaw(volatile uint16_t *buffer)
{
  float    k;
  uint16_t i;

  // Деление амплитуды сигнала на величину буфера
  // (для получения линейного возрастания)
  k = SIGNAL_AMPLITUDE / SIGNAL_SAMPLE_SIZE;

  // Цикл формирования сигнала
  for (i = 0; i < SIGNAL_SAMPLE_SIZE; i++)
     buffer[i] = VLT_TO_DGT(SIGNAL_CONSTANT + k * i);
}

// Отображение параметров сигнала на дисплей
void DAC_PrintInfo(void)
{
  // Сообщение, отображаемое на дисплей
  char message[LCD_STR_LEN];

  // Размер выборки
  snprintf(message, LCD_STR_LEN, "Выборка:   %d", SIGNAL_SAMPLE_SIZE);
  LCD_PutString(message, 3);

  // Частота сигнала
  snprintf(message, LCD_STR_LEN, "Частота:   %d Гц", SIGNAL_FREQUENCY);
  LCD_PutString(message, 4);

  // Постоянная составляющая сигнала
  snprintf(message, LCD_STR_LEN, "Конст.:    %.2f В", SIGNAL_CONSTANT);
  LCD_PutString(message, 5);

  // Амплитуда сигнала
  snprintf(message, LCD_STR_LEN, "Амплитуда: %.2f В", SIGNAL_AMPLITUDE);
  LCD_PutString(message, 6);
}
