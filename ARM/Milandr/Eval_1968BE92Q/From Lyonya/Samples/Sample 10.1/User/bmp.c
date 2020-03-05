/*******************************************************************************
 * @File: bmp.c
 * @Author: Milandr, L.
 * @Project: Sample 10.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 19.05.2017
 * @Purpose: ����� ������� � �������� �������� �������� � ����������� BMP280
 *******************************************************************************/

// ����������� ������������� �����
#include "bmp.h"

// ���������� ��� �������� ������������� ������
uint16_t dig_T1;
int16_t  dig_T2;
int16_t  dig_T3;
uint16_t dig_P1;
int16_t  dig_P2;
int16_t  dig_P3;
int16_t  dig_P4;
int16_t  dig_P5;
int16_t  dig_P6;
int16_t  dig_P7;
int16_t  dig_P8;
int16_t  dig_P9;

// ����������, ������������ ��� ����������� �������� �������� � ����������� �� �����������
BMP280_S32_t t_fine;

// ���������� ����������� � �������� ������� � ��������� 0.01 �C
// �������� �������� "5123" ������������ 51.23 �C
// ������� ����� �� ����������� ������������ �� ���������� BMP280
BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
  BMP280_S32_t var1, var2, T;
  var1 = ((((adc_T >> 3) - ((BMP280_S32_t)dig_T1 << 1))) * ((BMP280_S32_t)dig_T2)) >> 11;
  var2 = (((((adc_T >> 4) - ((BMP280_S32_t)dig_T1)) * ((adc_T >> 4) - ((BMP280_S32_t)dig_T1))) >> 12) * ((BMP280_S32_t)dig_T3)) >> 14;
  t_fine = var1 + var2;
  T = (t_fine * 5 + 128) >> 8;
  return T;
}

// ���������� �������� � ��������
// �������� �������� "96386" ������������ 96386 ��
// ������� ����� �� ����������� ������������ �� ���������� BMP280
BMP280_U32_t bmp280_compensate_P_int32(BMP280_S32_t adc_P)
{
  BMP280_S32_t var1, var2;
  BMP280_U32_t p;
  var1 = (((BMP280_S32_t)t_fine) >> 1) - (BMP280_S32_t)64000;
  var2 = (((var1 >> 2) * (var1 >> 2)) >> 11 ) * ((BMP280_S32_t)dig_P6);
  var2 = var2 + ((var1 * ((BMP280_S32_t)dig_P5)) << 1);
  var2 = (var2 >> 2) + (((BMP280_S32_t)dig_P4) << 16);
  var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13 )) >> 3) + ((((BMP280_S32_t)dig_P2) * var1) >> 1)) >> 18;
  var1 =((((32768 + var1)) * ((BMP280_S32_t)dig_P1)) >> 15);
  if (var1 == 0)
    return 0; // avoid exception caused by division by zero
  p = (((BMP280_U32_t)(((BMP280_S32_t)1048576) - adc_P) - (var2 >> 12))) * 3125;
  if (p < 0x80000000)
    p = (p << 1) / ((BMP280_U32_t)var1);
  else
    p = (p / (BMP280_U32_t)var1) * 2;
  var1 = (((BMP280_S32_t)dig_P9) * ((BMP280_S32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
  var2 = (((BMP280_S32_t)(p >> 2)) * ((BMP280_S32_t)dig_P8)) >> 13;
  p = (BMP280_U32_t)((BMP280_S32_t)p + ((var1 + var2 + dig_P7) >> 4));
  return p;
}

// ��������� ������������� ������ ���������� BMP280
void BMP280_GetCalibrationData(void)
{
  uint8_t data[6];  // ������ �������� ������

  // ��������� ������������� ������
  SPI_ReceiveDataBurst(MDR_SSP1, BMP280_CALIB_T, 6, data);

  // �������������� ���������� ������ � ������������� ���������
  dig_T1 = data[0] | (data[1] << 8);
  dig_T2 = data[2] | (data[3] << 8);
  dig_T3 = data[4] | (data[5] << 8);
}

// ������������� ���������� BMP280
void BMP280_Initialize(void)
{
  // ������������ ����� ��� ������ ���������� SPI
  SPI_PortCfg();

  // ��������� ������������� ������
  BMP280_GetCalibrationData();

  // ������������ ���������� BMP280
  // ��������� ��������� ������� � ��������� ���������
  SPI_TransmitData(MDR_SSP1, BMP280_CONFIG, BMP280_CONFIG_DATA);

  // ��������� �������� ��������� � ������ ������
  SPI_TransmitData(MDR_SSP1, BMP280_CTRL_MEAS, BMP280_CTRL_MEAS_DATA);
}

// ����� ������ � ����������� BMP280
void Thread_BMP280(void *argument)
{
  uint8_t data[3];            // ������ �������� ������
  uint32_t digitT;            // �������� �������� �����������
  float temperature;          // �����������
  char message[LCD_STR_LEN];  // ���������, ������������ �� �������

  // ������������� ���������� BMP280
  BMP280_Initialize();

  // �������� ���������� ������� ��������������
  osDelay(250);

  // �������� ����
  while (1)
  {
    // ������������ ����� �����-������ ��� ������ � ������� SSP1
    SPI_PortCfg();

    // ������ ������ �� ��������� �����������
    SPI_ReceiveDataBurst(MDR_SSP1, BMP280_TEMP_MSB, 3, data);

    // �������������� ���������� ������ � �������� �������� �����������
    digitT = (data[2] >> 4) | (data[1] << 4) | (data[0] << 12);

    // �������������� ��������� �������� ����������� � ������� �������
    temperature = (float)bmp280_compensate_T_int32(digitT) / 100;

    // ������������ ����� �����-������ ��� ������ � ��������
    LCD_PortCfg();

    // ����������� �������� �������� ����������� �� �������
    snprintf(message, LCD_STR_LEN, "    T = %.2f�C", temperature);
    LCD_PutString(message, 4);

    // �������� ����� ������� ��������� ��������
    osDelay(250);
  }
}
