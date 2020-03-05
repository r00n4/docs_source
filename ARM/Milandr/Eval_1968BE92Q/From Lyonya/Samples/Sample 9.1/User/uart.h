/*******************************************************************************
 * @File: uart.h
 * @Author: Milandr, L.
 * @Project: Sample 9.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 02.03.2017
 * @Purpose: ������ � ������������ UART
 *******************************************************************************/

#ifndef UART_H
#define UART_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� �������� �������� ������
#define UART_SPEED 115200

// ��������� �������
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx);  // ��������������� ����������� UART
void UART_PortCfg(void);                       // ������������ ����� ��� ������ � ������������ UART (UART2)
void UART_Init(void);                          // ������������� ������ UART (UART2)
void Thread_Typewriter(void *argument);        // ����� �������� �������� � ���������� �� �� ������� ���������� �����

#endif
