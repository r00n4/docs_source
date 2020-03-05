/*******************************************************************************
 * @File: tmr.c
 * @Author: Milandr, L.
 * @Project: Sample 5.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.03.2017
 * @Purpose: Конфигурация аппаратных таймеров
 *******************************************************************************/

// Подключение заголовочного файла
#include "tmr.h"

// Деинициализация аппаратного таймера
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx)
{
  MDR_TIMERx->CNT          = 0;
  MDR_TIMERx->PSG          = 0;
  MDR_TIMERx->ARR          = 0;
  MDR_TIMERx->CNTRL        = 0;
  MDR_TIMERx->CCR1         = 0;
  MDR_TIMERx->CCR2         = 0;
  MDR_TIMERx->CCR3         = 0;
  MDR_TIMERx->CCR4         = 0;
  MDR_TIMERx->CCR11        = 0;
  MDR_TIMERx->CCR21        = 0;
  MDR_TIMERx->CCR31        = 0;
  MDR_TIMERx->CCR41        = 0;
  MDR_TIMERx->CH1_CNTRL    = 0;
  MDR_TIMERx->CH2_CNTRL    = 0;
  MDR_TIMERx->CH3_CNTRL    = 0;
  MDR_TIMERx->CH4_CNTRL    = 0;
  MDR_TIMERx->CH1_CNTRL1   = 0;
  MDR_TIMERx->CH2_CNTRL1   = 0;
  MDR_TIMERx->CH3_CNTRL1   = 0;
  MDR_TIMERx->CH4_CNTRL1   = 0;
  MDR_TIMERx->CH1_CNTRL2   = 0;
  MDR_TIMERx->CH2_CNTRL2   = 0;
  MDR_TIMERx->CH3_CNTRL2   = 0;
  MDR_TIMERx->CH4_CNTRL2   = 0;
  MDR_TIMERx->CH1_DTG      = 0;
  MDR_TIMERx->CH2_DTG      = 0;
  MDR_TIMERx->CH3_DTG      = 0;
  MDR_TIMERx->CH4_DTG      = 0;
  MDR_TIMERx->BRKETR_CNTRL = 0;
  MDR_TIMERx->STATUS       = 0;
  MDR_TIMERx->IE           = 0;
  MDR_TIMERx->DMA_RE       = 0;
}

// Инициализация аппаратного таймера TIMER1
void TIMER_Init(void)
{
  // Включение тактирования таймера
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_TIMER1_Pos);
  MDR_RST_CLK->TIM_CLOCK |= (1 << RST_CLK_TIM_CLOCK_TIM1_CLK_EN_Pos);

  // Деинициализация таймера
  TIMER_Reset(MDR_TIMER1);

  // Предделитель тактовой частоты таймера
  MDR_TIMER1->PSG = 1 - 1;

  // Значение автоматической перезагрузки таймера
  MDR_TIMER1->ARR = SAMPLING_PERIOD(SIGNAL_FREQUENCY);

  // Общая конфигурация таймера
  MDR_TIMER1->CNTRL = (1 << TIMER_CNTRL_CNT_EN_Pos)      // Работа таймера (включен)
                    | (0 << TIMER_CNTRL_ARRB_EN_Pos)     // Режим обновления регистра ARR (мгновенное)
                    | (0 << TIMER_CNTRL_DIR_Pos)         // Направление счета (прямой счет)
                    | (0 << TIMER_CNTRL_FDTS_Pos)        // Частота выборки (каждый импульс)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)    // Режим счета (тактовые импульсы с фиксированным направлением)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);  // Источник событий для счета (передний фронт тактовых импульсов)

  // Разрешение запросов к контроллеру DMA
  MDR_TIMER1->DMA_RE = (1 << TIMER_DMA_RE_CNT_ARR_EVENT_RE_Pos);  // Запрос при CNT = ARR
}
