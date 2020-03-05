/*******************************************************************************
 * @File: i2c.h
 * @Author: Milandr, L.
 * @Project: Sample 11.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 08.02.2017
 * @Purpose: Работа с шиной I2C
 *******************************************************************************/

#ifndef I2C_H
#define I2C_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Требуемая скорость передачи по шине I2C (бит/с)
#define I2C_SPEED 100000

// Макрос для расчета предделителя, соответствующего требуемой скорости
#define I2C_PRESCALER (SystemCoreClock / (5 * I2C_SPEED) - 1)

// Макрос для расчета максимальнго числа попыток при опросе флагов контроллера I2C
// (в расчете используется время, требуемое для передачи 4 бит,
// а также количество тактов, требуемых на итерацию цикла (5))
#define I2C_MAX_ATTEMPTS (SystemCoreClock * 4 / (5 * I2C_SPEED))

// Макрос опроса флагов контроллера I2C
#define I2C_CHECK_ROUTINE(ERROR_FLAG) {                                  \
                                        attempt++;                       \
                                        if (attempt > I2C_MAX_ATTEMPTS)  \
                                        {                                \
                                          MDR_I2C->CMD = I2C_CMD_STOP;   \
                                          return ERROR_FLAG;             \
                                        }                                \
                                      }                                  \
                                      attempt = 0

// Псевдонимы ошибок работы с шиной I2C
enum I2C_ERRORS
{
  ERROR_OK = 0,         // Нет ошибки
  ERROR_BUS_IS_BUSY,    // Ошибка освобождения линии
  ERROR_NO_TRANSMIT,    // Ошибка передачи данных
  ERROR_NO_RECEIVE,     // Ошибка приема данных
  ERROR_NO_ACKNOWLEDGE  // Ошибка подтверждения
};

// Прототипы функций
void I2C_Reset(void);                                                                                           // Деинициализация контроллера I2C
void I2C_PortCfg(void);                                                                                         // Инициализация порта для работы линий SCL и SDA
void I2C_Init(void);                                                                                            // Инициализация контроллера I2C
uint8_t I2C_TransmitData(uint8_t i2c_address, uint8_t register_address, uint8_t data);                          // Отправление данных по шине I2C
uint8_t I2C_ReceiveData(uint8_t i2c_address, uint8_t register_address, uint8_t *data);                          // Получение данных по шине I2C
uint8_t I2C_ReceiveDataBurst (uint8_t i2c_address, uint8_t register_address, uint8_t amount, uint8_t *buffer);  // Последовательное получение нескольких пакетов данных
void I2C_ErrorHandler(uint8_t error);                                                                           // Обработчик ошибок шины I2C

#endif
