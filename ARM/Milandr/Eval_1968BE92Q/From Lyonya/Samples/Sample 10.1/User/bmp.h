/*******************************************************************************
 * @File: bmp.h
 * @Author: Milandr, L.
 * @Project: Sample 10.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 19.05.2017
 * @Purpose: ����� ������� � �������� �������� �������� � ����������� BMP280
 *******************************************************************************/

#ifndef BMP_H
#define BMP_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ���� ������, ������������ � ������� ���
#define BMP280_S32_t int32_t
#define BMP280_U32_t uint32_t

// ������ ��������� ���������� BMP280
#define BMP280_CALIB_T    0x88
#define BMP280_CALIB_P    0x8E
#define BMP280_ID         0xD0
#define BMP280_RESET      0xE0
#define BMP280_STATUS     0xF3
#define BMP280_CTRL_MEAS  0xF4
#define BMP280_CONFIG     0xF5
#define BMP280_PRESS_MSB  0xF7
#define BMP280_PRESS_LSB  0xF8
#define BMP280_PRESS_XLSB 0xF9
#define BMP280_TEMP_MSB   0xFA
#define BMP280_TEMP_LSB   0xFB
#define BMP280_TEMP_XLSB  0xFC

// ������������ ����������� ��������� ���������� BMP280
// ������� CTRL_MEAS
#define MODE     0x3  // ����� ������ ����������
#define OSRS_P   0x0  // ������� ������������� ��� ��������� ��������
#define OSRS_T   0x1  // ������� ������������� ��� ��������� �����������

// ������� CONFIG
#define SPI3W_EN 0x0  // ����� �������� ������ �� ���� ���������� ������
#define FILTER   0x0  // ������� �������� ����������
#define T_SB     0x3  // ��������� �������� ����� �����������

// ������������ ���������������� ������
#define BMP280_CTRL_MEAS_DATA  MODE | (OSRS_P << 2) | (OSRS_T << 5)
#define BMP280_CONFIG_DATA     SPI3W_EN | (FILTER << 2) | (T_SB << 5)

// ��������� �������
BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T);  // ���������� �����������
BMP280_U32_t bmp280_compensate_P_int32(BMP280_S32_t adc_P);  // ���������� ��������
void BMP280_GetCalibrationData(void);                        // ��������� ������������� ������ ���������� BMP280
void BMP280_Initialize(void);                                // ������������� ���������� BMP280
void Thread_BMP280(void *argument);                          // ����� ������ ���������� BMP280

#endif
