/*******************************************************************************
 * @File: can.c
 * @Author: Milandr, L.
 * @Project: Sample 12.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.08.2017
 * @Purpose: Работа с модулем CAN
 *******************************************************************************/
 
// Подключение заголовочного файла
#include "can.h"

// Деинициализация контроллера CAN
void CAN_Reset(MDR_CAN_TypeDef *MDR_CANx)
{
  uint32_t i;

  MDR_CANx->CONTROL = 0;
  MDR_CANx->STATUS  = 0;
  MDR_CANx->BITTMNG = 0;
  MDR_CANx->INT_EN  = 0;
  MDR_CANx->OVER    = 0;
  MDR_CANx->INT_RX  = 0;
  MDR_CANx->INT_TX  = 0;

  // Сброс регистров управления буферами
  for (i = 0; i < 32; i++)
    MDR_CANx->BUF_CON[i] = 0;

  // Сброс фильтров
  for (i = 0; i < 32; i++)
  {
    MDR_CANx->CAN_BUF_FILTER[i].FILTER = 0;
    MDR_CANx->CAN_BUF_FILTER[i].MASK   = 0;
  }
}

// Конфигурация порта для работы контроллера CAN1
void CAN_PortCfg(void)
{
  // Включение тактирования порта A
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTA_Pos);

  // Конфигурация линий PA6 (TX) и PA7 (RX)
  MDR_PORTA->OE     |=  ((1 << 6));               // Направление передачи данных (PA6 - вывод)
  MDR_PORTA->OE     &= ~((1 << 7));               // Направление передачи данных (PA7 - ввод)
  MDR_PORTA->FUNC   &= ~((3 << 12) | (3 << 14));  // Сброс битов регистра FUNC
  MDR_PORTA->FUNC   |=  ((2 << 12) | (2 << 14));  // Функция линий (альтернативная)
  MDR_PORTA->ANALOG |=  ((1 << 6)  | (1 << 7));   // Режим работы линий (цифровой)
  MDR_PORTA->PULL   &= ~((1 << 6)  | (1 << 7));   // Резисторы подтяжки к цепи питания (отключены)
  MDR_PORTA->PULL   &= ~((1 << 22) | (1 << 23));  // Резисторы подтяжки к земле (отключены)
  MDR_PORTA->PD     &= ~((1 << 6)  | (1 << 7));   // Управление линиями (драйвер)
  MDR_PORTA->PD     &= ~((1 << 22) | (1 << 23));  // Триггеры Шмитта (отключены)
  MDR_PORTA->PWR    |=  ((3 << 12) | (3 << 14));  // Длительность фронтов импульсов (низкая)
  MDR_PORTA->GFEN   &= ~((1 << 6)  | (1 << 7));   // Цифровые фильтры (отключены)
}

// Инициализация контроллера CAN (CAN1)
void CAN_Init(void)
{
  // Конфигурация порта для работы контроллера CAN
  CAN_PortCfg();

  // Включение тактирования контроллера CAN
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_CAN1_Pos);
  MDR_RST_CLK->CAN_CLOCK |= (1 << RST_CLK_CAN_CLOCK_CAN1_CLK_EN_Pos);

  // Деинициализация контроллера CAN
  CAN_Reset(MDR_CAN1);

  // Конфигурация контроллера CAN
  MDR_CAN1->CONTROL = (1 << 0)   // Работа контроллера (включен)
                    | (0 << 1)   // Режим "только чтение" (отключен)
                    | (1 << 2)   // Режим самотестирования (включен)
                    | (1 << 3)   // Подтв. приема собственных кадров (включено)
                    | (1 << 4);  // Прием собственных кадров (включен)

  // Конфигурация битового интервала
  MDR_CAN1->BITTMNG = (15 << 0)   // Делитель тактовой частоты (16 – 1)
                    | (1 << 16)   // Длительность сегмента распространения (2 TQ)
                    | (3 << 19)   // Длительность первого фазового сегмента (4 TQ)
                    | (2 << 22)   // Длительность второго фазового сегмента (3 TQ)
                    | (1 << 25)   // Допустимый интервал подстройки сегментов (2 TQ)
                    | (0 << 27);  // Режим выборки (однократная)

  // Конфигурация буферов
  // Буфер приема
  MDR_CAN1->BUF_CON[RX_BUF_NUM] = (1 << 0)   // Работа буфера (включен)
                                | (1 << 1)   // Режим работы буфера (прием)
                                | (0 << 2)   // Разрешение перезаписи кадра (запрещено)
                                | (0 << 3)   // Разрешение ответа на удаленный запрос (не используется)
                                | (1 << 4);  // Приоритет буфера (обычный)

  // Буфер передачи
  MDR_CAN1->BUF_CON[TX_BUF_NUM] = (1 << 0)   // Работа буфера (включен)
                                | (0 << 1)   // Режим работы буфера (передача)
                                | (0 << 2)   // Разрешение перезаписи кадра (не используется)
                                | (0 << 3)   // Разрешение ответа на удаленный запрос (запрещен)
                                | (1 << 4);  // Приоритет буфера (обычный)

  // Настройка запросов на обработку прерываний от контроллера CAN
  MDR_CAN1->INT_EN = (1 << 0)   // Общий флаг запросов
                   | (1 << 1);  // Прерывание при приеме кадра

  // Настройка запросов на обработку прерываний при приеме кадра в определенный буфер
  MDR_CAN1->INT_RX = (1 << RX_BUF_NUM);

  // Назначение приоритета прерывания от контроллера CAN
  NVIC_SetPriority(CAN1_IRQn, 1);

  // Разрешение обработки прерывания от контроллера CAN
  NVIC_EnableIRQ(CAN1_IRQn);
}

// Извлечение принятого кадра из буфера
CAN_FrameTypeDef CAN_ExtractFrame(uint8_t buffer_number)
{
  CAN_FrameTypeDef frame;

  frame.ID    = MDR_CAN1->CAN_BUF[buffer_number].ID;
  frame.IDE   = MDR_CAN1->CAN_BUF[buffer_number].DLC >> 12 & 0x1;
  frame.DLC   = MDR_CAN1->CAN_BUF[buffer_number].DLC & 0xF;
  frame.DATAL = MDR_CAN1->CAN_BUF[buffer_number].DATAL;
  frame.DATAH = MDR_CAN1->CAN_BUF[buffer_number].DATAH;

  return frame;
}

// Поток передатчика CAN
void Thread_Transmitter(void *argument)
{
  // Начальное значение идентификатора
  uint32_t id = 234;

  // Основной цикл
  while (1)
  {
    // Проверка нажатия кнопки SEL
    if ((MDR_PORTC->RXTX & (1 << 2)) == 0)
    {
      // Формирование кадра
      MDR_CAN1->CAN_BUF[TX_BUF_NUM].ID    = id;          // Идентификатор кадра
      MDR_CAN1->CAN_BUF[TX_BUF_NUM].DLC   = (1 << 0)     // Биты DLC (1 байт)
                                          | (0 << 8)     // Бит RTR (нет запроса)
                                          | (1 << 9)     // Бит R1 (всегда должен быть равен 1)
                                          | (0 << 10)    // Бит R0 (всегда должен быть равен 0)
                                          | (1 << 11)    // Бит SSR (всегда должен быть равен 1)
                                          | (1 << 12);   // Бит IDE (расширенный формат)
      MDR_CAN1->CAN_BUF[TX_BUF_NUM].DATAL = 0x000000AA;  // Байты данных 1...4
      MDR_CAN1->CAN_BUF[TX_BUF_NUM].DATAH = 0x00000000;  // Байты данных 5...8

      // Запрос на передачу сформированного кадра
      MDR_CAN1->BUF_CON[TX_BUF_NUM] |= (1 << 5);

      // Ожидание отпускания кнопки
      while ((MDR_PORTC->RXTX & (1 << 2)) == 0)
        osDelay(25);

      // Инкремент идентификатора
      id++;
    }

    // Задержка перед началом следующей проверки
    osDelay(25);
  }
}

// Поток приемника CAN
void Thread_Receiver(void *argument)
{
  CAN_FrameTypeDef rx_frame;  // Структура принимаемого кадра
  char message[LCD_STR_LEN];  // Сообщение, отображаемое на дисплей

  // Вывод на дисплей сообщения об ожидании
  LCD_PutString("   Ожидание кадра ", 4);

  // Основной цикл
  while (1)
  {
    // Ожидание сообщения с принятыми кадров
    osMessageQueueGet(MsgQueId_CAN, &rx_frame, NULL, osWaitForever);

    // Отображение содержания кадра на дисплей
    sprintf(message, "ID    = %d", rx_frame.ID);
    LCD_PutString(message, 3);
    sprintf(message, "IDE   = %d", rx_frame.IDE);
    LCD_PutString(message, 4);
    sprintf(message, "DLC   = %d", rx_frame.DLC);
    LCD_PutString(message, 5);
    sprintf(message, "DATAL = %d", rx_frame.DATAL);
    LCD_PutString(message, 6);
    sprintf(message, "DATAH = %d", rx_frame.DATAH);
    LCD_PutString(message, 7);
  }
}

// Обработчик прерываний от контроллера CAN1
void CAN1_IRQHandler(void)
{
  // Структура принимаемого кадра
  CAN_FrameTypeDef rx_frame;

  // Извлечение данных из буфера
  rx_frame = CAN_ExtractFrame(RX_BUF_NUM);

  // Передача данных
  osMessageQueuePut(MsgQueId_CAN, &rx_frame, osPriorityNormal, 0);

  // Сброс флага прерывания при приеме кадра
  MDR_CAN1->BUF_CON[RX_BUF_NUM] &= ~(1 << 6);
}

void SomeFunc(uint8_t sum)
{
  uint8_t mass[sum];
}