/*******************************************************************************
 * @File: i2c.c
 * @Author: Milandr, L.
 * @Project: Sample 11.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 08.02.2017
 * @Purpose: ������ � ����� I2C
 *******************************************************************************/
 
// ����������� ������������� �����
#include "i2c.h"

// ��������������� ����������� I2C
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

// ������������� ����� ��� ������ ����������� I2C
void I2C_PortCfg(void)
{
  // ������������ ����� PC0 � PC1 ��� ������ � �������� ����� SCL � SDA
  MDR_PORTC->OE     &= ~((1 << 0)  | (1 << 1));   // ����������� ������ (�� ������������)
  MDR_PORTC->FUNC   &= ~((3 << 0)  | (3 << 2));   // ����� ����� �������� FUNC
  MDR_PORTC->FUNC   |=  ((2 << 0)  | (2 << 2));   // ������� ����� (��������������)
  MDR_PORTC->ANALOG |=  ((1 << 0)  | (1 << 1));   // ����� ������ ����� (��������)
  MDR_PORTC->PULL   &= ~((1 << 0)  | (1 << 1));   // �������� � ����� (���������)
  MDR_PORTC->PULL   &= ~((1 << 16) | (1 << 17));  // �������� � ����� (���������)
  MDR_PORTC->PD     &= ~((1 << 0)  | (1 << 1));   // ���������� ������� (�� ������������)
  MDR_PORTC->PD     &= ~((1 << 16) | (1 << 17));  // �������� ������ (�� ������������)
  MDR_PORTC->PWR    |=  ((3 << 0)  | (3 << 2));   // �������� ������� (�������)
  MDR_PORTC->GFEN   &=  ((1 << 0)  | (1 << 1));   // �������� ������� (�� ������������)
}

// ������������� ����������� I2C
void I2C_Init(void)
{
  // ��������� ������������ ����� C
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTC_Pos);

  // ������������ ����� ��� ������ ����������� I2C
  I2C_PortCfg();

  // ��������� ������������ ����������� I2C
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_I2C_Pos);

  // ��������������� ����������� I2C
  I2C_Reset();

  // ������������ ����������� I2C
  MDR_I2C->PRL = I2C_PRESCALER;              // ������� 8 ��� ������������ �������� �������
  MDR_I2C->PRH = I2C_PRESCALER >> 8;         // ������� 8 ��� ������������ �������� �������
  MDR_I2C->CTR = (0 << I2C_CTR_S_I2C_Pos)    // ���������� �������� �������� (400 ����/�)
               | (0 << I2C_CTR_EN_INT_Pos)   // ���������� �� ����������� I2C (���������)
               | (1 << I2C_CTR_EN_I2C_Pos);  // ��������� ����������� I2C
}

// ����������� ������ �� ���� I2C
uint8_t I2C_TransmitData(uint8_t i2c_address,       // ����� ���������� �� ���� I2C
                         uint8_t register_address,  // ����� �������� ������ ����������
                         uint8_t data)              // ������������ ������
{
  // ���������� ��� ������� ���������� �������
  uint32_t attempt;

  // �������� ������������ ���� I2C
  while (MDR_I2C->STA & I2C_STA_BUSY)
    I2C_CHECK_ROUTINE(ERROR_BUS_IS_BUSY);

  // ������� ������ ���������� � �������� ������ ������ (��� 0 = R/W = 0)
  MDR_I2C->TXD = (i2c_address << 1);

  // ����������� �������� START � ��������� ��������
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // �������� ���������� ��������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // �������� ���� �������������
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // ����������� ������ ��������
  MDR_I2C->TXD = register_address;
  MDR_I2C->CMD = I2C_CMD_WR;

  // �������� ���������� ��������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // �������� ��������� ���� �������������
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // ����������� ������
  MDR_I2C->TXD = data;
  MDR_I2C->CMD = I2C_CMD_WR;

  // �������� ���������� ��������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // �������� ��������� ���� �������������
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // ����������� �������� STOP
  MDR_I2C->CMD = I2C_CMD_STOP;
  return ERROR_OK;
}

// ��������� ������ �� ���� I2C
uint8_t I2C_ReceiveData(uint8_t i2c_address,       // ����� ���������� �� ���� I2C
                        uint8_t register_address,  // ����� �������� ������ ����������
                        uint8_t *data)             // ���������� ��� �������� ������
{
  // ���������� ��� ������� ���������� �������
  uint32_t attempt;

  // �������� ������������ ���� I2C
  while (MDR_I2C->STA & I2C_STA_BUSY)
    I2C_CHECK_ROUTINE(ERROR_BUS_IS_BUSY);

  // ������� ������ ���������� � �������� ������ ������ (��� 0 = R/W = 0)
  MDR_I2C->TXD = (i2c_address << 1);

  // ����������� �������� START � ��������� ��������
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // �������� ���������� ��������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // �������� ��������� ���� �������������
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // ����������� ������ ��������
  MDR_I2C->TXD = register_address;
  MDR_I2C->CMD = I2C_CMD_WR;

  // �������� ���������� ��������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // �������� ��������� ���� �������������
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // ������� ������ ���������� � �������� ������ ������ (��� 0 = R/W = 1)
  MDR_I2C->TXD = (i2c_address << 1) | 0x01;

  // ����������� ���������� �������� START � ��������� ��������
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // �������� ���������� ��������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // �������� ��������� ���� �������������
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // ����� ������ � ����������� ���� NACK ��� ���������� ��������
  MDR_I2C->CMD = I2C_CMD_RD | I2C_CMD_ACK;

  // �������� ���������� ������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_RECEIVE);

  // ������ �������� ������
  *data = MDR_I2C->RXD;

  // ����������� �������� STOP
  MDR_I2C->CMD = I2C_CMD_STOP;
  return ERROR_OK;
}

// ���������������� ��������� ���������� ������� ������
uint8_t I2C_ReceiveDataBurst(uint8_t i2c_address,       // ����� ���������� �� ���� I2C
                             uint8_t register_address,  // ����� ���������� �������� ������ ����������
                             uint8_t amount,            // ���������� ������ ������
                             uint8_t *buffer)           // ����� ��� �������� ����������� ������
{
  // ���������� ��� ������� ���������� �������
  uint32_t attempt;

  // �������� ������������ ���������
  if (amount < 1)
    return ERROR_OK;

  // �������� ������������ ���� I2C
  while (MDR_I2C->STA & I2C_STA_BUSY)
    I2C_CHECK_ROUTINE(ERROR_BUS_IS_BUSY);

  // ������� ������ ���������� � �������� ������ ������ (��� 0 = R/W = 0)
  MDR_I2C->TXD = (i2c_address << 1);

  // ����������� �������� START � ��������� ��������
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // �������� ���������� ��������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // �������� ��������� ���� �������������
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // ����������� ������ ���������� ��������
  MDR_I2C->TXD = register_address;
  MDR_I2C->CMD = I2C_CMD_WR;

  // �������� ���������� ��������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // �������� ��������� ���� �������������
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // ������� ������ ���������� � �������� ������ ������ (��� 0 = R/W = 1)
  MDR_I2C->TXD = (i2c_address << 1) | 0x01;

  // ����������� ���������� �������� START � ��������� ��������
  MDR_I2C->CMD = I2C_CMD_START | I2C_CMD_WR;

  // �������� ���������� ��������
  while (MDR_I2C->STA & I2C_STA_TR_PROG)
    I2C_CHECK_ROUTINE(ERROR_NO_TRANSMIT);

  // �������� ��������� ���� �������������
  if (MDR_I2C->STA & I2C_STA_RX_ACK)
    return ERROR_NO_ACKNOWLEDGE;

  // ���� ������ �������� ���������, �� ����������� ����������
  for (uint16_t i = 0; i < amount - 1; i++)
  {
    // ����� ������ � ����������� ���� ACK
    MDR_I2C->CMD = I2C_CMD_RD & ~I2C_CMD_ACK;

    // �������� ���������� ������
    while (MDR_I2C->STA & I2C_STA_TR_PROG)
      I2C_CHECK_ROUTINE(ERROR_NO_RECEIVE);

    // ������ �������� ������ � �����
    buffer[i] = MDR_I2C->RXD;
  }

  // ������ ���������� ��������
  // ����� ������ � ����������� ���� NACK ��� ���������� ��������
  MDR_I2C->CMD = I2C_CMD_RD | I2C_CMD_ACK;

  // �������� ���������� ������
    while (MDR_I2C->STA & I2C_STA_TR_PROG)
      I2C_CHECK_ROUTINE(ERROR_NO_RECEIVE);

  // ������ �������� ������ � �����
  buffer[amount - 1] = MDR_I2C->RXD;

  // ����������� �������� STOP
  MDR_I2C->CMD = I2C_CMD_STOP;
  return ERROR_OK;
}

// ���������� ������ ���� I2C
void I2C_ErrorHandler(uint8_t error)
{
  // ������������ ����� �����-������ ��� ������ �������
  LCD_PortCfg();

  switch (error)
  {
    // ��������� ������ ������������ ����
    case (ERROR_BUS_IS_BUSY):
      LCD_PutString("     ���� ������", 7);
    break;

    // ��������� ������ �������� ������
    case (ERROR_NO_TRANSMIT):
      LCD_PutString("   ������ ��������", 7);
    break;

    // ��������� ������ ������ ������
    case (ERROR_NO_RECEIVE):
      LCD_PutString("    ������ ������", 7);
    break;

    // ��������� ������ �������������
    case (ERROR_NO_ACKNOWLEDGE):
      LCD_PutString("������ �������������", 7);
    break;
  }
}
