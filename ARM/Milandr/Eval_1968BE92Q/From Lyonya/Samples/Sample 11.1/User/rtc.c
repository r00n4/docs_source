/*******************************************************************************
 * @File: rtc.c
 * @Author: Milandr, L.
 * @Project: Sample 11.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 08.02.2017
 * @Purpose: ������ � RTC DS1337
 *******************************************************************************/

// ����������� ������������� �����
#include "rtc.h"

// ��������� ������� � ����
void RTC_SetTime (void)
{
  uint8_t i;

  // ������ ��������� ������� � ����
  uint8_t time[7] = {SECONDS, MINUTES, HOURS, DAY, DATE, MONTH, YEAR};

   // ������������ ����� �����-������ ��� ������ ����������� I2C
  I2C_PortCfg();

  // ���� ��������� ������� � ����
  for (i = 0; i < 7; i++)
  {
    // �������������� ������ � �������-���������� ������
    time[i] = (time[i] / 10) << 4 | (time[i] % 10);

    // ����������� ������
    I2C_TransmitData(RTC_I2C_ADDRESS, RTC_REGISTER_SECONDS + i, time[i]);
  }
}

// �������������� ������ � ���������� ������
uint8_t RTC_ConvertData(uint8_t register_address, uint8_t value)
{
  // �������������� ������ ��� ������� �� ������������ ���������
  switch (register_address)
  {
    // ������� ������
    case RTC_REGISTER_SECONDS:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x07);

    // ������� �����
    case RTC_REGISTER_MINUTES:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x07);

    // ������� �����
    case RTC_REGISTER_HOURS:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x03);

    // ������� ��� ������
    case RTC_REGISTER_DAY:
      return value & 0x07;

    // ������� �����
    case RTC_REGISTER_DATE:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x03);

    // ������� ������
    case RTC_REGISTER_MONTH:
      return (value & 0x0F) + 10 * ((value >> 4) & 0x01);

    // ������� ����
    case RTC_REGISTER_YEAR:
      return (value & 0x0F) + 10 * (value >> 4);

    // �� ��������� - ��� ���������
    default:
      return value;
  }
}

// ����� ����������� �������
void Thread_DisplayTime(void *argument)
{
  uint8_t time[7];            // ������ ��� �������� ���� � �������
  char message[LCD_STR_LEN];  // ���������, ������������ �� �������
  uint8_t error;              // ���������� ��� �������� ���� ������

  // �������� ����
  while (1)
  {
    // ������������ ����� �����-������ ��� ������ ����������� I2C
    I2C_PortCfg();

    // ������ ��������� ������� � ����
    error = I2C_ReceiveDataBurst(RTC_I2C_ADDRESS, RTC_REGISTER_SECONDS, 7, time);

    // ���� � �������� ������ � ����� ������������� ������...
    if (error != 0)
    {
      // ������ ��������� ��������� ������
      I2C_ErrorHandler(error);
    }

    // � ��������� ������� ������� ��������� �������� ������
    else
    {
      // �������������� ���������� ������ � ���������� ������
      for (uint8_t i = 0; i < 7; i++)
        time[i] = RTC_ConvertData(RTC_REGISTER_SECONDS + i, time[i]);

      // ������������ ����� �����-������ ��� ������ �������
      LCD_PortCfg();

      // �������� ������ � ����������� �� �������
      LCD_ClearString(7);

      // ����������� ������� �� �������
      snprintf(message, LCD_STR_LEN, "      %02d:%02d:%02d", time[2], time[1], time[0]);
      LCD_PutString (message, 5);

      // ����������� ���� �� �������
      snprintf(message, LCD_STR_LEN, "     %02d.%02d.20%02d", time[4], time[5], time[6]);
      LCD_PutString(message, 3);
    }

    // �������� ����� ������� ��������� ��������
    osDelay(1000);
  }
}

// ����� ��������� �������
void Thread_SetTime(void *argument)
{
  // �������� ����
  while (1)
  {
    // �������� �������
    if ((MDR_PORTC->RXTX & (1 << 2)) == 0)
    {
      // �������� ��� ������ �� �������� ���������
      osDelay(10);

      // ������������� �������
      if ((MDR_PORTC->RXTX & (1 << 2)) == 0)
      {
        // ��������� ��������� �������
        RTC_SetTime();

        // �������� ����������
        while ((MDR_PORTC->RXTX & (1 << 2)) == 0)
          osDelay(25);
      }
    }

    // �������� ����� ��������� ���������
    osDelay(25);
  }
}
