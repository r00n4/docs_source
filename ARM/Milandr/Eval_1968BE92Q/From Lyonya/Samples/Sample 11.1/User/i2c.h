/*******************************************************************************
 * @File: i2c.h
 * @Author: Milandr, L.
 * @Project: Sample 11.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 08.02.2017
 * @Purpose: ������ � ����� I2C
 *******************************************************************************/

#ifndef I2C_H
#define I2C_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� �������� �������� �� ���� I2C (���/�)
#define I2C_SPEED 100000

// ������ ��� ������� ������������, ���������������� ��������� ��������
#define I2C_PRESCALER (SystemCoreClock / (5 * I2C_SPEED) - 1)

// ������ ��� ������� ������������ ����� ������� ��� ������ ������ ����������� I2C
// (� ������� ������������ �����, ��������� ��� �������� 4 ���,
// � ����� ���������� ������, ��������� �� �������� ����� (5))
#define I2C_MAX_ATTEMPTS (SystemCoreClock * 4 / (5 * I2C_SPEED))

// ������ ������ ������ ����������� I2C
#define I2C_CHECK_ROUTINE(ERROR_FLAG) {                                  \
                                        attempt++;                       \
                                        if (attempt > I2C_MAX_ATTEMPTS)  \
                                        {                                \
                                          MDR_I2C->CMD = I2C_CMD_STOP;   \
                                          return ERROR_FLAG;             \
                                        }                                \
                                      }                                  \
                                      attempt = 0

// ���������� ������ ������ � ����� I2C
enum I2C_ERRORS
{
  ERROR_OK = 0,         // ��� ������
  ERROR_BUS_IS_BUSY,    // ������ ������������ �����
  ERROR_NO_TRANSMIT,    // ������ �������� ������
  ERROR_NO_RECEIVE,     // ������ ������ ������
  ERROR_NO_ACKNOWLEDGE  // ������ �������������
};

// ��������� �������
void I2C_Reset(void);                                                                                           // ��������������� ����������� I2C
void I2C_PortCfg(void);                                                                                         // ������������� ����� ��� ������ ����� SCL � SDA
void I2C_Init(void);                                                                                            // ������������� ����������� I2C
uint8_t I2C_TransmitData(uint8_t i2c_address, uint8_t register_address, uint8_t data);                          // ����������� ������ �� ���� I2C
uint8_t I2C_ReceiveData(uint8_t i2c_address, uint8_t register_address, uint8_t *data);                          // ��������� ������ �� ���� I2C
uint8_t I2C_ReceiveDataBurst (uint8_t i2c_address, uint8_t register_address, uint8_t amount, uint8_t *buffer);  // ���������������� ��������� ���������� ������� ������
void I2C_ErrorHandler(uint8_t error);                                                                           // ���������� ������ ���� I2C

#endif
