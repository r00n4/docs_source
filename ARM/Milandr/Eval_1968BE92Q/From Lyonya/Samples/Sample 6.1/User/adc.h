/*******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: Sample 6.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 24.03.2017
 * @Purpose: ���������� �������-�������� ����������������
 *******************************************************************************/

#ifndef ADC_H
#define ADC_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ������������� ��������� ����������
#define CALIBR_CONST_V 3.290F
#define CALIBR_CONST_D 0xFFF

// ������ ��� ������� ����������
#define VOLTAGE(D) CALIBR_CONST_V * (D) / CALIBR_CONST_D

// ��������� �������
void ADC_Reset(void);             // ��������������� �������-�������� ����������������
void ADC_PortCfg(void);           // ������������ ����� ��� ������ ���
void ADC_Init(void);              // ������������� ���
void Thread_ADC(void *argument);  // ����� �������-�������� ��������������

#endif
