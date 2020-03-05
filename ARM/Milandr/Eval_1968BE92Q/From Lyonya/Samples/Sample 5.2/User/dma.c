/*******************************************************************************
 * @File: dma.c
 * @Author: Milandr, L.
 * @Project: Sample 5.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
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

  // Формирование основной и альтернативной структур управляющих данных канала 10 (TIMER1)
  DMA_ControlData[10].DMA_Control = DMA_ControlData[42].DMA_Control =
    (3 << DMA_CHANNEL_CFG_CYCLE_CTRL_Pos)                        // Режим работы контроллера DMA (пинг-понг)
  | (0 << DMA_CHANNEL_CFG_NEXT_USEBURST_Pos)                     // Режим пакетной передачи (отключен)
  | ((SIGNAL_SAMPLE_SIZE - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos)  // Количество передач в цикле (по размеру выборки)
  | (0 << DMA_CHANNEL_CFG_R_POWER_Pos)                           // Количество передач до выполнения арбитража (1)
  | (1 << DMA_CHANNEL_CFG_SRC_PROT_CTRL_Pos)                     // Режим защиты источника (привилегированный)
  | (1 << DMA_CHANNEL_CFG_DST_PROT_CTRL_Pos)                     // Режим защиты приемника (привилегированный)
  | (1 << DMA_CHANNEL_CFG_SRC_SIZE_Pos)                          // Размерность данных источника (2 байта)
  | (1 << DMA_CHANNEL_CFG_SRC_INC_Pos)                           // Инкремент адреса источника данных (2 байта)
  | (1 << DMA_CHANNEL_CFG_DST_SIZE_Pos)                          // Размерность данных приемника (2 байта)
  | (3U << DMA_CHANNEL_CFG_DST_INC_Pos);                         // Инкремент адреса приемника данных (отсутствует)

  // Формирование указателя конца данны источника
  DMA_ControlData[10].DMA_SourceEndPointer = DMA_ControlData[42].DMA_SourceEndPointer = (uint32_t)&signal + ((SIGNAL_SAMPLE_SIZE - 1) << 1);

  // Формирование указателя конца данны приемника
  DMA_ControlData[10].DMA_DestinationEndPointer = DMA_ControlData[42].DMA_DestinationEndPointer = (uint32_t)&MDR_DAC->DAC2_DATA;

  // Задание базового адреса управляющих данных
  MDR_DMA->CTRL_BASE_PTR = (uint32_t)&DMA_ControlData;

  // Конфигурация контроллера DMA
  MDR_DMA->CFG = (1 << DMA_CFG_MASTER_ENABLE_Pos)    // Работа контроллера DMA (включен)
               | (1 << DMA_CFG_CHNL_PROT_CTRL_Pos);  // Режим защиты шины (привилегированный)

  // Разрешение работы канала 10
  MDR_DMA->CHNL_ENABLE_SET = (1 << 10);

  // Использование *основной* структуры управления для канала 10
  MDR_DMA->CHNL_PRI_ALT_CLR = (1 << 10);

  // Приоритет канала 10 (обычный)
  MDR_DMA->CHNL_PRIORITY_CLR = (1 << 10);

  // Назначение приоритета прерывания от контроллера DMA
  NVIC_SetPriority(DMA_IRQn, 1);

  // Разрешение обработки прерывания от контроллера DMA
  NVIC_EnableIRQ(DMA_IRQn); 
}

// Обработчик аппаратного прерывания от DMA
void DMA_IRQHandler(void)
{
  if ((DMA_ControlData[10].DMA_Control & DMA_CHANNEL_CFG_CYCLE_CTRL_Msk) == 0)
  {
    // Формирование нового цикла передач
    // путем модификации полей структуры управляющих данных
    DMA_ControlData[10].DMA_Control |= (3 << DMA_CHANNEL_CFG_CYCLE_CTRL_Pos)                        // Режим работы (пинг-понг)
                                    | ((SIGNAL_SAMPLE_SIZE - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos);  // Количество передач в цикле
  }

  if ((DMA_ControlData[42].DMA_Control & DMA_CHANNEL_CFG_CYCLE_CTRL_Msk) == 0)
  {
    // Формирование нового цикла передач
    // путем модификации полей структуры управляющих данных
    DMA_ControlData[42].DMA_Control |= (3 << DMA_CHANNEL_CFG_CYCLE_CTRL_Pos)                        // Режим работы (пинг-понг)
                                    | ((SIGNAL_SAMPLE_SIZE - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos);  // Количество передач в цикле
  }

  // Разрешение работы канала 10
  MDR_DMA->CHNL_ENABLE_SET = (1 << 10);
}
