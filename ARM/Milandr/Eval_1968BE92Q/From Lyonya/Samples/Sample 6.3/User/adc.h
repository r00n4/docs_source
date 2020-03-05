/*******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: Sample 6.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: ���������� �������-�������� ����������������
 *******************************************************************************/

#ifndef ADC_H
#define ADC_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ������������� ��������� ��� ����������
#define ADC_SAMPLE_SIZE 64

// ����� ��� �������� ����������� ��������������
extern volatile uint32_t sample[ADC_SAMPLE_SIZE];

// ��������� �������
void ADC_Reset(void);             // ��������������� �������-�������� ����������������
void ADC_PortCfg(void);           // ������������ ����� ��� ������ ���
void ADC_Init(void);              // ������������� �������-��������� ���������������
void Thread_ADC(void *argument);  // ����� �������-�������� ��������������

#endif
