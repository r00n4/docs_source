/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 7.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 10.12.2018
 * @Purpose: ������������ ���������� ��������
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// �������� �������� ������� ������� TIMER1
#define TIMER1_PRESCALER 1

// ������� �������-���������� ��������� (� ��)
#define PULSE_FREQUENCY 100000

// ������ ��� ������� ������� ���������
#define PULSE_PERIOD(F) (uint16_t)(SystemCoreClock / ((uint32_t)(F) * TIMER1_PRESCALER) - 1)

// ������ ��� ������� ������������ ����������

// #define DUTY_CYCLE(D) (uint16_t)(PULSE_PERIOD * (D) / 0xFFF)

// ��������� �������
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // ��������������� �������
void TIMER_PortCfg(void);                         // ������������ ����� ��� ������ � ��������
void TIMER_Init(void);                            // ������������� ������� (TIMER1)

#endif
