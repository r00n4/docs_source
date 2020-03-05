/*******************************************************************************
 * @File: dac.h
 * @Author: Milandr, L.
 * @Project: Sample 5.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: ���������� �����-���������� ����������������
 *******************************************************************************/

#ifndef DAC_H
#define DAC_H

// ����������� ����������� ���������
#include "link.h"

// ����� ��
#define PI 3.1415926535F

// ������� ���������� ��� (� �������)
#define DAC_REF 3.290F

// ������������ �������� ���� ���
#define DAC_MAX 4095

// ������ ��� �������� ���������� (� �������) � ��� ���
#define VLT_TO_DGT(V) (uint16_t)(((V) / DAC_REF) * DAC_MAX)

// ���������� �������� �� ������ �������
#define SIGNAL_SAMPLE_SIZE 128

// �������������� �������
#define SIGNAL_CONSTANT    1.00F   // ���������� ������������ ������� (� �������)
#define SIGNAL_AMPLITUDE   0.75F   // ��������� ������� (� �������)
#define SIGNAL_FREQUENCY   2000    // ������� ������� (� ��)

// ������ �������� �������
extern volatile uint16_t signal[SIGNAL_SAMPLE_SIZE];

// ��������� �������
void DAC_Reset(void);                           // ��������������� �����-����������� ���������������
void DAC_PortCfg(void);                         // ������������ ����� � �������� ������ ���
void DAC_Init(void);                            // ������������� �����-����������� ���������������
void GenerateSinus(volatile uint16_t *buffer);  // ��������� ��������������� �������
void GenerateSaw(volatile uint16_t *buffer);    // ��������� ������������� �������
void DAC_PrintInfo(void);                       // ����������� ���������� ������� �� �������

#endif
