/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 5.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.03.2017
 * @Purpose: ������������ ���������� ��������
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ������ ��� �������� ������� ������������� �� �������� ������� �������
#define SAMPLING_PERIOD(F) (uint16_t)(SystemCoreClock / (uint32_t)((F) * SIGNAL_SAMPLE_SIZE) - 1)

// ��������� �������
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // ��������������� ����������� �������
void TIMER_Init(void);                            // ������������� ����������� ������� TIMER1

#endif
