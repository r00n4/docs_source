/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 3.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 13.03.2019
 * @Purpose: ������������ ���������� ��������/���������
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// �������� �������� ������� TIMER1
#define PRESCALER 620

// ��������� �������
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // ��������������� ����������� �������
void TIMER_Init(void);                            // ������������� ����������� �������

#endif
