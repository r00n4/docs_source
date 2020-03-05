/*******************************************************************************
 * @File: dma.c
 * @Author: Milandr, L.
 * @Project: Sample 7.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: Конфигурация прямого доступа к памяти
 *******************************************************************************/

// Подключение заголовочного файла
#include "dma.h"

// Структуры управляющих данных для контроллера DMA
// 32 канала; основная и альтернативная структуры; итого - 64 элемента
// Структура состоит из четырех 32-битных полей; итого - 1024 байта
volatile DMA_ControlDataTypeDef DMA_ControlData[64] __attribute__ ((aligned (1024)));

// Деинициализация контроллера DMA
void DMA_Reset(void)
{
  MDR_DMA->CFG               = 0;
  MDR_DMA->CTRL_BASE_PTR     = 0;
  MDR_DMA->CHNL_SW_REQUEST   = 0;
  MDR_DMA->CHNL_USEBURST_CLR = 0xFFFFFFFF;
  MDR_DMA->CHNL_REQ_MASK_CLR = 0xFFFFFFFF;
  MDR_DMA->CHNL_ENABLE_CLR   = 0xFFFFFFFF;
  MDR_DMA->CHNL_PRI_ALT_CLR  = 0xFFFFFFFF;
  MDR_DMA->CHNL_PRIORITY_CLR = 0xFFFFFFFF;
  MDR_DMA->ERR_CLR           = 1;
}

// Инициализация контроллера DMA
void DMA_Init(void)
{
  // Включение тактирования контроллера DMA
  // Для корректной работы контроллера DMA также необходимо включить тактирование контроллеров SSP1 и SSP2
  // (если этого не сделать, то постоянно будет возникать прерывание от контроллера DMA)
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_DMA_Pos)    // Контроллер DMA
                          | (1 << RST_CLK_PCLK_SSP1_Pos)   // Контроллер SSP1
                          | (1 << RST_CLK_PCLK_SSP2_Pos);  // Контроллер SSP2

  // Сброс прерывания от контроллера DMA
  NVIC_ClearPendingIRQ(DMA_IRQn);

  // Деинициализация контроллера DMA
  DMA_Reset();

  // Формирование управляющих данных канала 8 (ADC1)
  DMA_ControlData[8].DMA_Control = (1 << DMA_CHANNEL_CFG_CYCLE_CTRL_Pos)                     // Режим работы контроллера DMA (основной)
                                 | (0 << DMA_CHANNEL_CFG_NEXT_USEBURST_Pos)                  // Режим пакетной передачи (отключен)
                                 | ((ADC_SAMPLE_SIZE - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos)  // Количество передач в цикле (по размеру выборки)
                                 | (0 << DMA_CHANNEL_CFG_R_POWER_Pos)                        // Количество передач до выполнения арбитража (1)
                                 | (1 << DMA_CHANNEL_CFG_SRC_PROT_CTRL_Pos)                  // Режим защиты источника (привилегированный)
                                 | (1 << DMA_CHANNEL_CFG_DST_PROT_CTRL_Pos)                  // Режим защиты приемника (привилегированный)
                                 | (1 << DMA_CHANNEL_CFG_SRC_SIZE_Pos)                       // Размерность данных источника (2 байта)
                                 | (3 << DMA_CHANNEL_CFG_SRC_INC_Pos)                        // Инкремент адреса источника данных (отсутствует)
                                 | (1 << DMA_CHANNEL_CFG_DST_SIZE_Pos)                       // Размерность данных приемника (2 байта)
                                 | (1U << DMA_CHANNEL_CFG_DST_INC_Pos);                      // Инкремент адреса приемника данных (2 байта)

  // Формирование указателя конца данны источника
  DMA_ControlData[8].DMA_SourceEndPointer = (uint32_t)&MDR_ADC->ADC1_RESULT;

  // Формирование указателя конца данны приемника
  DMA_ControlData[8].DMA_DestinationEndPointer = (uint32_t)&sample + ((ADC_SAMPLE_SIZE - 1) << 1);

  // Задание базового адреса управляющих данных
  MDR_DMA->CTRL_BASE_PTR = (uint32_t)&DMA_ControlData;

  // Конфигурация контроллера DMA
  MDR_DMA->CFG = (1 << DMA_CFG_MASTER_ENABLE_Pos)    // Работа контроллера DMA (включен)
               | (1 << DMA_CFG_CHNL_PROT_CTRL_Pos);  // Режим защиты шины (привилегированный)

  // Разрешение работы канала 8
  MDR_DMA->CHNL_ENABLE_SET = (1 << 8);

  // Использование *основной* структуры управления для канала 8
  MDR_DMA->CHNL_PRI_ALT_CLR = (1 << 8);

  // Приоритет канала 8 (обычный)
  MDR_DMA->CHNL_PRIORITY_CLR = (1 << 8);

  // Назначение приоритета прерывания от контроллера DMA
  NVIC_SetPriority(DMA_IRQn, 1);

  // Разрешение обработки прерывания от контроллера DMA
  NVIC_EnableIRQ(DMA_IRQn); 
}

// Обработчик аппаратного прерывания от DMA
void DMA_IRQHandler(void)
{
  // Отключение АЦП
  MDR_ADC->ADC1_CFG &= ~(1 << ADC1_CFG_REG_ADON_Pos);

  // Установка флага завершения цикла передач
  osEventFlagsSet(EventId_ADC, EVENT_ADC_EOC);

  // Формирование нового цикла передач
  // путем модификации полей структуры управляющих данных
  DMA_ControlData[8].DMA_Control |= (1 << DMA_CHANNEL_CFG_CYCLE_CTRL_Pos)                     // Режим работы (основной)
                                 | ((ADC_SAMPLE_SIZE - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos);  // Количество передач в цикле

  // Разрешение работы канала 8
  MDR_DMA->CHNL_ENABLE_SET = (1 << 8);
}
