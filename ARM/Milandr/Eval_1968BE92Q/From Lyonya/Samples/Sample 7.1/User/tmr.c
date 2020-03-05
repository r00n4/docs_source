/*******************************************************************************
 * @File: tmr.c
 * @Author: Milandr, L.
 * @Project: Sample 7.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: Конфигурация аппаратных таймеров
 *******************************************************************************/

// Подключение заголовочного файла
#include "tmr.h"

// Коэффициент заполнения импульсов (в процентах)
int8_t duty_cycle = 50;

// Деинициализация таймера
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

// Конфигурация порта для работы с таймером
void TIMER_PortCfg(void)
{
  // Включение тактирования порта F
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTF_Pos);

  // Конфигурация линии PF6 для работы в качестве канала таймера
  MDR_PORTF->OE     |=  (1 << 6);   // Направление линии (вывод)
  MDR_PORTF->FUNC   &= ~(3 << 12);  // Сброс битов регистра FUNC
  MDR_PORTF->FUNC   |=  (2 << 12);  // Функция линии (альтернативная)
  MDR_PORTF->ANALOG |=  (1 << 6);   // Режим работы линии (цифровой)
  MDR_PORTF->PULL   &= ~(1 << 6);   // Подтяжка к цепи питания (отключена)
  MDR_PORTF->PULL   &= ~(1 << 22);  // Подтяжка к земле (отключена)
  MDR_PORTF->PD     &= ~(1 << 6);   // Управление линией (драйвер)
  MDR_PORTF->PD     &= ~(1 << 22);  // Триггер Шмитта (отключен)
  MDR_PORTF->PWR    |=  (3 << 12);  // Крутизна импульсов (высокая)
  MDR_PORTF->GFEN   &= ~(1 << 6);   // Цифровой фильтр (отключен)
}

// Инициализация таймера в режиме широтно-импульсной модуляции
void TIMER_Init(void)
{
  // Включение тактирования таймера
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_TIMER1_Pos);
  MDR_RST_CLK->TIM_CLOCK |= (1 << RST_CLK_TIM_CLOCK_TIM1_CLK_EN_Pos);

  // Деинициализация таймера
  TIMER_Reset(MDR_TIMER1);

  // Конфигурация порта для работы с таймером
  TIMER_PortCfg();

  // Предделитель тактовой частоты таймера
  MDR_TIMER1->PSG = TIMER1_PRESCALER - 1;

  // Значение автоматической перезагрузки таймера
  MDR_TIMER1->ARR = PULSE_PERIOD(PULSE_FREQUENCY);

  // Общая конфигурация таймера
  MDR_TIMER1->CNTRL = (0 << TIMER_CNTRL_CNT_EN_Pos)             // Работа таймера (пока отключен)
                    | (1 << TIMER_CNTRL_ARRB_EN_Pos)            // Режим обновления регистра ARR (при перезагрузке)
                    | (0 << TIMER_CNTRL_DIR_Pos)                // Направление счета (прямой счет)
                    | (0 << TIMER_CNTRL_FDTS_Pos)               // Частота выборки (не используется)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)           // Режим счета (тактовые импульсы с фиксированным направлением)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);         // Источник событий для счета (передний фронт тактовых импульсов)

  // Общая конфигурация канала CH1 таймера
  MDR_TIMER1->CH1_CNTRL  = (0 << TIMER_CH_CNTRL_CHFLTR_Pos)     // Фильтрация входного сигнала (не используется)
                         | (0 << TIMER_CH_CNTRL_CHSEL_Pos)      // Событие для фиксации счетчика в регистр CCR (не используется)
                         | (0 << TIMER_CH_CNTRL_CHPSC_Pos)      // Предделитель частоты входного сигнала (не используется)
                         | (0 << TIMER_CH_CNTRL_OCCE_Pos)       // Использование канала ETR (не используется)
                         | (6 << TIMER_CH_CNTRL_OCCM_Pos)       // Формат выходного сигнала (REF) (CHOUT = {!DIR при CNT <  CCR,
                                                                //                                           DIR при CCR <= CNT <= ARR})
                         | (0 << TIMER_CH_CNTRL_BRKEN_Pos)      // Сброс по выводу BRK (не используется)
                         | (0 << TIMER_CH_CNTRL_ETREN_Pos)      // Сброс по выводу ETR (не используется)
                         | (0 << TIMER_CH_CNTRL_CAP_NPWM_Pos);  // Режим работы канала (ШИМ)

  // Конфигурация линий канала таймера
  // Прямая линия
  MDR_TIMER1->CH1_CNTRL1 = (1 << TIMER_CH_CNTRL1_SELOE_Pos)     // Разрешение генерации выходного сигнала на прямой линии (разрешена)
                         | (2 << TIMER_CH_CNTRL1_SELO_Pos)      // Управление выходом прямого линии (REF)
                         | (0 << TIMER_CH_CNTRL1_INV_Pos)       // Инверсия прямого линии (отсутствует)

  // ...Инверсная линия
                         | (0 << TIMER_CH_CNTRL1_NSELOE_Pos)    // Разрешение генерации выходного сигнала на инверсной линии (запрещена)
                         | (0 << TIMER_CH_CNTRL1_NSELO_Pos)     // Управление выходом инверсной линии (отключено)
                         | (0 << TIMER_CH_CNTRL1_NINV_Pos);     // Инверсия инверсной линии (отсутствует)

  // Конфигурация процесса сравнения
  MDR_TIMER1->CH1_CNTRL2 = (0 << TIMER_CH_CNTRL2_CHSEL1_Pos)    // Событие для фиксации значения регистра CCR1 (не используется)
                         | (0 << TIMER_CH_CNTRL2_CCR1_EN_Pos)   // Использование регистра CCR1 (не используется)
                         | (1 << TIMER_CH_CNTRL2_CCRRLD_Pos);   // Обновление регистров CCR и CCR1 (при перезагрузке)

  // Задание исходного значения коэффициента заполнения импульсов
  PWM_SetDutyCycle(duty_cycle);

  // Запуск таймера
  MDR_TIMER1->CNTRL |= (1 << TIMER_CNTRL_CNT_EN_Pos);
}

// Задание скважности импульсов
void PWM_SetDutyCycle(uint8_t duty_cycle)
{
  // Сообщение, отображаемое на дисплей
  char message[LCD_STR_LEN];

  // Вычисление ширины импульсов (в отсчетах таймера),
  // и запись вычисленного значения в регистр сравнения CCR
  MDR_TIMER1->CCR1 = PULSE_PERIOD(PULSE_FREQUENCY) * duty_cycle / 100;

  // Если требуется получить 100-процентный коэффициент заполнения,
  // то берется значение CCR на единицу больше периода,
  // чтобы исключить однотактные провалы сигнала
  if (duty_cycle == 100)
    MDR_TIMER1->CCR1 = PULSE_PERIOD(PULSE_FREQUENCY) + 1;

  // Отображение заданного коэфициента заполнения на дисплей
  snprintf(message, LCD_STR_LEN, "       D = %u%%", duty_cycle);
  LCD_PutString(message, 4);
}
