/*******************************************************************************
 * @File: i2c.c
 * @Author: Milandr, L.
 * @Project: Sample 11.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 08.02.2017
 * @Purpose: Работа с шиной I2C
 *******************************************************************************/
 
// Подключение заголовочного файла
#include "i2c.h"

// Деинициализация контроллера I2C
void I2C_Reset(void)
{
  MDR_I2C->PRL = 0;
  MDR_I2C->PRH = 0;
  MDR_I2C->CTR = 0;
  MDR_I2C->RXD = 0;
  MDR_I2C->STA = 0;
  MDR_I2C->TXD = 0;
  MDR_I2C->CMD = 0;
}

// Инициализация порта для работы контроллера I2C
void I2C_PortCfg(void)
{
  // Конфигурация линий PC0 и PC1 для работы в качестве линий SCL и SDA
  MDR_PORTC->OE     &= ~((1 << 0)  | (1 << 1));   // Направление данных (не используется)
  MDR_PORTC->FUNC   &= ~((3 << 0)  | (3 << 2));   // Сброс битов регистра FUNC
  MDR_PORTC->FUNC   |=  ((2 << 0)  | (2 << 2));   // Функция линий (альтернативная)
  MDR_PORTC->ANALOG |=  ((1 << 0)  | (1 << 1));   // Режим работы линии (цифровой)
  MDR_PORTC->PULL   &= ~((1 << 0)  | (1 << 1));   // Подтяжка к земле (отключены)
  MDR_PORTC->PULL   &= ~((1 << 16) | (1 << 17));  // Подтяжка к земле (отключены)
  MDR_PORTC->PD     &= ~((1 << 0)  | (1 << 1));   // Управление линиями (не используется)
  MDR_PORTC->PD     &= ~((1 << 16) | (1 << 17));  // Триггеры Шмитта (не используются)
  MDR_PORTC->PWR    |=  ((3 << 0)  | (3 << 2));   // Крутизна фронтов (высокая)
  MDR_PORTC->GFEN   &=  ((1 << 0)  | (1 << 1));   // Цифровые фильтры (не используются)
}

// Инициализация контроллера I2C
void I2C_Init(void)
{
  // Включение тактирования порта C
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTC_Pos);

  // Конфигурация порта для работы контроллера I2C
  I2C_PortCfg();

  // Включение тактирования контроллера I2C
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_I2C_Pos);

  // Деинициализация контроллера I2C
  I2C_Reset();

  // Конфигурация контроллера I2C
  MDR_I2C->PRL = I2C_PRESCALER;              // Младшие 8 бит предделителя тактовой частоты
  MDR_I2C->PRH = I2C_PRESCALER >> 8;         // Старшие 8 бит предделителя тактовой частоты
  MDR_I2C->CTR = (0 << I2C_CTR_S_I2C_Pos)    // Предельная скорость передачи (400 Кбит/с)
               | (0 << I2C_CTR_EN_INT_Pos)   // Прерывание от контроллера I2C (запрещено)
               | (1 << I2C_CTR_EN_I2C_Pos);  // Включение контроллера I2C
}

// Отправление данных по шине I2C
uint8_t I2C_TransmitData(uint8_t i2c_address,       // Адрес устройства на шине I2C
                         uint8_t register_address,  // Адрес регистра памяти устройства
                         uint8_t data)              // Передаваемые данные
{
  // Переменная для отсчета количества попыток
  uint32_t attempt;

  // Ожидание освобождения шины I2C
  while (MDR_I2C->STA & I2C_STA_BUSY)
    I2C_CHECK_ROUTINE(ERROR_BUS_IS_BUSY);

  // Задание адреса устройства и признака ЗАПИСИ данных (бит 0 = R/W = 0)
  MDR_I2C->TXD = (i2c_address << 1);

  // Отправление признака START и инициация передачи
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // Ожидание завершения передачи
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // Проверка бита подтверждения
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // Отправление адреса регистра
  MDR_I2C->TXD = register_address;
  MDR_I2C->CMD = I2C_CMD_WR;

  // Ожидание завершения передачи
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // Проверка получения бита подтверждения
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // Отправление данных
  MDR_I2C->TXD = data;
  MDR_I2C->CMD = I2C_CMD_WR;

  // Ожидание завершения передачи
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // Проверка получения бита подтверждения
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // Отправление признака STOP
  MDR_I2C->CMD = I2C_CMD_STOP;
  return ERROR_OK;
}

// Получение данных по шине I2C
uint8_t I2C_ReceiveData(uint8_t i2c_address,       // Адрес устройства на шине I2C
                        uint8_t register_address,  // Адрес регистра памяти устройства
                        uint8_t *data)             // Переменная для хранения данных
{
  // Переменная для отсчета количества попыток
  uint32_t attempt;

  // Ожидание освобождения шины I2C
  while (MDR_I2C->STA & I2C_STA_BUSY)
    I2C_CHECK_ROUTINE(ERROR_BUS_IS_BUSY);

  // Задание адреса устройства и признака ЗАПИСИ данных (бит 0 = R/W = 0)
  MDR_I2C->TXD = (i2c_address << 1);

  // Отправление признака START и инициация передачи
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // Ожидание завершения передачи
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // Проверка получения бита подтверждения
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // Отправление адреса регистра
  MDR_I2C->TXD = register_address;
  MDR_I2C->CMD = I2C_CMD_WR;

  // Ожидание завершения передачи
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // Проверка получения бита подтверждения
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // Задание адреса устройства и признака ЧТЕНИЯ данных (бит 0 = R/W = 1)
  MDR_I2C->TXD = (i2c_address << 1) | 0x01;

  // Отправление повторного признака START и инициация передачи
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // Ожидание завершения передачи
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // Проверка получения бита подтверждения
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // Прием данных и отправление бита NACK для завершения передачи
  MDR_I2C->CMD = I2C_CMD_RD | I2C_CMD_ACK;

  // Ожидание завершения приема
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_RECEIVE);

  // Чтение принятых данных
  *data = MDR_I2C->RXD;

  // Отправление признака STOP
  MDR_I2C->CMD = I2C_CMD_STOP;
  return ERROR_OK;
}

// Последовательное получение нескольких пакетов данных
uint8_t I2C_ReceiveDataBurst(uint8_t i2c_address,       // Адрес устройства на шине I2C
                             uint8_t register_address,  // Адрес начального регистра памяти устройства
                             uint8_t amount,            // Количество циклов чтения
                             uint8_t *buffer)           // Буфер для хранения результатов чтения
{
  // Переменная для отсчета количества попыток
  uint32_t attempt;

  // Проверка корректности аргумента
  if (amount < 1)
    return ERROR_OK;

  // Ожидание освобождения шины I2C
  while (MDR_I2C->STA & I2C_STA_BUSY)
    I2C_CHECK_ROUTINE(ERROR_BUS_IS_BUSY);

  // Задание адреса устройства и признака ЗАПИСИ данных (бит 0 = R/W = 0)
  MDR_I2C->TXD = (i2c_address << 1);

  // Отправление признака START и инициация передачи
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // Ожидание завершения передачи
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // Проверка получения бита подтверждения
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // Отправление адреса начального регистра
  MDR_I2C->TXD = register_address;
  MDR_I2C->CMD = I2C_CMD_WR;

  // Ожидание завершения передачи
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // Проверка получения бита подтверждения
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // Задание адреса устройства и признака ЧТЕНИЯ данных (бит 0 = R/W = 1)
  MDR_I2C->TXD = (i2c_address << 1) | 0x01;

  // Отправление повторного признака START и инициация передачи
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // Ожидание завершения передачи
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // Проверка получения бита подтверждения
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // Цикл чтения заданных регистров, за исключением последнего
  for (uint16_t i = 0; i < amount - 1; i++)
  {
    // Прием данных и отправление бита ACK
    MDR_I2C->CMD = I2C_CMD_RD & ~I2C_CMD_ACK;

    // Ожидание завершения приема
    while (MDR_I2C->STA & I2C_STA_TR_PROG)
      I2C_CHECK_ROUTINE(ERROR_NO_RECEIVE);

    // Запись принятых данных в буфер
    buffer[i] = MDR_I2C->RXD;
  }

  // Чтение последнего регистра
  // Прием данных и отправление бита NACK для завершения передачи
  MDR_I2C->CMD = I2C_CMD_RD | I2C_CMD_ACK;

  // Ожидание завершения приема
    while (MDR_I2C->STA & I2C_STA_TR_PROG)
      I2C_CHECK_ROUTINE(ERROR_NO_RECEIVE);

  // Запись принятых данных в буфер
  buffer[amount - 1] = MDR_I2C->RXD;

  // Отправление признака STOP
  MDR_I2C->CMD = I2C_CMD_STOP;
  return ERROR_OK;
}

// Обработчик ошибок шины I2C
void I2C_ErrorHandler(uint8_t error)
{
  // Конфигурация линий ввода-вывода для работы дисплея
  LCD_PortCfg();

  switch (error)
  {
    // Обработка ошибки освобождения шины
    case (ERROR_BUS_IS_BUSY):
      LCD_PutString("     Шина занята", 7);
    break;

    // Обработка ошибки передачи данных
    case (ERROR_NO_TRANSMIT):
      LCD_PutString("   Ошибка передачи", 7);
    break;

    // Обработка ошибки приема данных
    case (ERROR_NO_RECEIVE):
      LCD_PutString("    Ошибка приема", 7);
    break;

    // Обработка ошибки подтверждения
    case (ERROR_NO_ACKNOWLEDGE):
      LCD_PutString("Ошибка подтверждения", 7);
    break;
  }
}
