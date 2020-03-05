/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 3.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 15.03.2019
 * @Purpose: ������������ ���������� ��������/���������
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ������������ �������� ������� TIMER1
#define TIMER1_PRESCALER 36622

// ������ ��� �������������� �������� ������� �� ����� (� ��������)
#define CNT_TO_TIME(CNT) ((float)(CNT) * TIMER1_PRESCALER / SystemCoreClock)

// ��������� �������
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // ��������������� ����������� �������
void TIMER_Init(void);                            // ������������� ����������� �������
void Thread_TimeCounter(void *argument);          // ����� ����������� �������

#endif
