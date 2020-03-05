/*******************************************************************************
 * @File: rtos.h
 * @Author: Milandr, L.
 * @Project: Sample 6.4
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: ���������� ������������ �������� ��������� �������
 *******************************************************************************/

#ifndef RTOS_H
#define RTOS_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� ����� ���������� �������-��������� ��������������
#define EVENT_ADC_EOC 0x00000001

// ������������� ������� �������
extern osEventFlagsId_t EventId_ADC;

// �������������� ����������� ��������
extern osTimerId_t TimerId_MoveDoggie;
extern osTimerId_t TimerId_TimeCounter;
extern osTimerId_t TimerId_GameOver;

// ��������� �������
void RTOS_Init(void);  // ������������� ������������ ������� ��������� �������

#endif
