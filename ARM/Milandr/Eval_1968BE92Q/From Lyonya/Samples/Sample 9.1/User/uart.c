/*******************************************************************************
 * @File: uart.c
 * @Author: Milandr, L.
 * @Project: Sample 9.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 02.03.2017
 * @Purpose: Работа с контроллером UART
 *******************************************************************************/
 
// Подключение заголовочного файла
#include "uart.h"

// Деинициализация контроллера UART
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx)
{
  MDR_UARTx->RSR_ECR = 0;
  MDR_UARTx->FR      = UART_FR_RXFE   // Сброс всех флагов, кроме флагов
                     | UART_FR_TXFE;  // отсутствия данных в приемнике и передатчике
  MDR_UARTx->ILPR    = 0;
  MDR_UARTx->IBRD    = 0;
  MDR_UARTx->FBRD    = 0;
  MDR_UARTx->LCR_H   = 0;
  MDR_UARTx->CR      = 0;
  MDR_UARTx->IFLS    = 0;
  MDR_UARTx->IMSC    = 0;
  MDR_UARTx->DMACR   = 0;
}

// Конфигурация порта для работы контроллера UART (UART2)
void UART_PortCfg(void)
{
  // Конфигурации линий PF0 (RX) и PF1 (TX)
  MDR_PORTF->OE     &= ~((1 << 0));               // Направление данных (PF0 - ввод)
  MDR_PORTF->OE     |=  ((1 << 1));               // Направление данных (PF1 - вывод)
  MDR_PORTF->PULL   &= ~((1 << 0)  | (1 << 1));   // Подтяжка к земле (отключена)
  MDR_PORTF->PULL   &= ~((1 << 16) | (1 << 17));  // Подтяжка к питанию (отключена)
  MDR_PORTF->ANALOG |=  ((1 << 0)  | (1 << 1));   // Режим работы линий (цифровой)
  MDR_PORTF->FUNC   |=  ((3 << 0)  | (3 << 2));   // Функция линий (переопределенная)
  MDR_PORTF->PD     &= ~((1 << 0)  | (1 << 1));   // Управление линиями (не используется)
  MDR_PORTF->PD     &= ~((1 << 16) | (1 << 17));  // Триггеры Шмитта (не используются)
  MDR_PORTF->PWR    |=  ((3 << 0)  | (3 << 2));   // Крутизна фронтов (высокая)
  MDR_PORTF->GFEN   &= ~((1 << 0)  | (1 << 1));   // Цифровые фильтры (не используются)
}

// Инициализация модуля UART (UART2)
void UART_Init(void)
{
  // Включение тактирования порта F
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTF_Pos);

  // Конфигурация порта для работы контроллера UART
  UART_PortCfg();

  // Включение тактирования контроллера UART2
  MDR_RST_CLK->PER_CLOCK  |= (1 << RST_CLK_PCLK_UART2_Pos);
  MDR_RST_CLK->UART_CLOCK |= (1 << RST_CLK_UART_CLOCK_UART2_CLK_EN_Pos);

  // Деинициализация контроллера UART
  UART_Reset(MDR_UART2);

  // Задание скорости передачи данных
  // Вычисление требуемого значения делителя тактовой частоты
  uint32_t divider  = SystemCoreClock / (UART_SPEED >> 2);

  // Запись в регистр целой части делителя
  MDR_UART2->IBRD = divider >> 6;

  // Запись в регистр дробной части делителя
  MDR_UART2->FBRD = divider & 0x3F;

  // Конфигурация формата данных
  MDR_UART2->LCR_H = (1 << UART_LCR_H_PEN_Pos)   // Использование контрольного бита (включено)
                   | (1 << UART_LCR_H_EPS_Pos)   // Проверка на четность или нечетность (четность)
                   | (0 << UART_LCR_H_STP2_Pos)  // Количество STOP-битов (один)
                   | (0 << UART_LCR_H_FEN_Pos)   // Использование буфера FIFO (не используется)
                   | (3 << UART_LCR_H_WLEN_Pos)  // Количество битов данных (8)
                   | (0 << UART_LCR_H_SPS_Pos);  // Фиксация значения контрольного бита (отключена)

  // Конфигурация контроллера UART
  MDR_UART2->CR = (1 << UART_CR_UARTEN_Pos)  // Работа контроллера UART (включен)
                | (0 << UART_CR_SIREN_Pos)   // Работа ИК-кодека (отключен)
                | (0 << UART_CR_SIRLP_Pos)   // Длительность импульсов при ИК-передаче (не используется)
                | (0 << UART_CR_LBE_Pos)     // Режим самотестирования (отключен)
                | (1 << UART_CR_TXE_Pos)     // Разрешение передачи (разрешена)
                | (1 << UART_CR_RXE_Pos)     // Разрешение приема (разрешен)
                | (0 << UART_CR_DTR_Pos)     // Инверсия сигнала на линии DTR (не используется)
                | (0 << UART_CR_RTS_Pos)     // Инверсия сигнала на линии RTS (не используется)
                | (0 << UART_CR_OUT1_Pos)    // Инверсия сигнала на линии OUT1 (не используется)
                | (0 << UART_CR_OUT2_Pos)    // Инверсия сигнала на линии OUT2 (не используется)
                | (0 << UART_CR_RTSEN_Pos)   // Использование линии RTS (не используется)
                | (0 << UART_CR_CTSEN_Pos);  // Использование линии CTS (не используется)

  // Настройка запросов на обработку прерываний от контроллера UART
  MDR_UART2->IMSC = (1 << UART_IMSC_RXIM_Pos);  // Прерывание от приемника

  // Назначение приоритета прерывания от контроллера UART
  NVIC_SetPriority(UART2_IRQn, 1);

  // Разрешение обработки прерывания от контроллера UART
  NVIC_EnableIRQ(UART2_IRQn);
}

// Отображение символов с инкрементом координат
void LCD_PrintSymbol(char symbol)
{
  static int8_t x, y;  // Координаты символа

  // Обработка команд
  switch(symbol)
  {
    // Обработка операции стирания (Backspace / Delete)
    case 0x08:
    case 0x7F:
      LCD_PutSymbol(' ', x, y);
      x--;
      if (x < 0)
      {
        x = 20;
        y--;
        if (y < 0)
          x = y = 0;
      }
      LCD_PutSymbol('_', x, y);
      break;

    // Обработка операции перехода на новую строку (Enter)
    case 0x0D:
      LCD_PutSymbol(' ', x, y);
      x = 0;
      y++;
      if (y > 7)
        y = 0;
      LCD_PutSymbol('_', x, y);
      break;

    // Обработка операции очистки дисплея (Escape)
    case 0x1B:
      LCD_ClearScreen();
      LCD_PutSymbol('_', 0, 0);
      x = y = 0;
      break;

    // Обработка символов
    default:
    LCD_PutSymbol(symbol, x, y);
    x++;
    if (x > 20)
    {
      x = 0;
      y++;
      if (y > 7)
        y = 0;
    }
    LCD_PutSymbol('_', x, y);
    break;
  }
}

// Поток передачи символов с клавиатуры ПК на дисплей отладочной платы
void Thread_Typewriter(void *argument)
{
  // Переменная для хранения полученных данных
  uint8_t data;

  // Отображение указателя на дисплей
  LCD_PutSymbol('_', 0, 0);

  // Основной цикл
  while(1)
  {
    // Ожидание приема данных
    osEventFlagsWait(EventId_UART, EVENT_DATA_RECEIVED, osFlagsWaitAny, osWaitForever);

    // Считывание принятых данных
    data = MDR_UART2->DR;

    // Отображение принятого символа на дисплей
    LCD_PrintSymbol(data);

    // Передача данных в обратном направлении
    MDR_UART2->DR = data;
  }
}

// Обработчик прерываний от контроллера UART2
void UART2_IRQHandler(void)
{
    // Сброс запроса на обработку прерывания
    MDR_UART2->ICR |= (1 << UART_ICR_RXIC_Pos);

    // Установка флага завершения приема данных
    osEventFlagsSet(EventId_UART, EVENT_DATA_RECEIVED);
}
