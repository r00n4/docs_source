/*******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: Sample 7.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: ������������ ���������� ��������
 *******************************************************************************/

#ifndef TMR_H
#define TMR_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// �������� �������� ������� ������� TIMER1
#define TIMER1_PRESCALER 10

// ������� �������-���������� ��������� (� ��)
#define PULSE_FREQUENCY 10000

// ��� ��������� ������������ ����������
#define DUTY_CYCLE_DELTA 10

// ������ ��� ������� ������� ���������
#define PULSE_PERIOD(F) (uint16_t)(SystemCoreClock / ((uint32_t)(F) * TIMER1_PRESCALER) - 1)

// ����������� ���������� ���������
extern int8_t duty_cycle;

// ��������� �������
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);  // ��������������� �������
void TIMER_PortCfg(void);                         // ������������ ����� ��� ������ � ��������
void TIMER_Init(void);                            // ������������� ������� (TIMER1)
void PWM_SetDutyCycle(uint8_t duty_cycle);        // ������� ���������� ���������

#endif
