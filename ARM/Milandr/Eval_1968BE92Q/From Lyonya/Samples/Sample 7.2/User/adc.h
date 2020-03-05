/*******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: Sample 7.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 10.12.2018
 * @Purpose: ���������� �������-�������� ����������������
 *******************************************************************************/

#ifndef ADC_H
#define ADC_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ������ �������
#define ADC_SAMPLE_SIZE 128

// ������������ �������� �������-��������� ��������������
#define D_MAX 0xFFF

// ����� ��� �������� ����������� ��������������
extern volatile uint16_t sample[ADC_SAMPLE_SIZE];

// ��������� �������
void ADC_Reset(void);                                  // ��������������� �������-�������� ����������������
void ADC_PortCfg(void);                                // ������������ ����� ��� ������ ���
void ADC_Init(void);                                   // ������������� ���
uint32_t ADC_MedianFilter(volatile uint16_t *buffer);  // ��������� ����������
void Thread_ADC(void *argument);                       // ����� �������-�������� ��������������

#endif
