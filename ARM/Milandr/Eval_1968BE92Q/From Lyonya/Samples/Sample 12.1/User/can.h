/*******************************************************************************
 * @File: can.h
 * @Author: Milandr, L.
 * @Project: Sample 12.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.08.2017
 * @Purpose: ������ � ������� CAN
 *******************************************************************************/

#ifndef CAN_H
#define CAN_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ������ ������� ������ CAN ��� ������ � ��������
#define RX_BUF_NUM 0
#define TX_BUF_NUM 1

// ������� � ������ ������
#define EVENT_CAN_DATA_RECEIVER 0x00000001

// ��������� �����
typedef struct
{
  uint32_t ID;        // �������������
  uint8_t  IDE;       // ������ �����
  uint8_t  DLC;       // ��� ����� ������
  uint32_t DATAL;     // ����� ������ 1...4
  uint32_t DATAH;     // ����� ������ 5...8
} CAN_FrameTypeDef;

// ��������� �������
void CAN_Reset(MDR_CAN_TypeDef *MDR_CANx);                 // ��������������� ����������� CAN
void CAN_PortCfg(void);                                    // ������������ ����� ��� ������ ����������� CAN (CAN1)
void CAN_Init(void);                                       // ������������� ����������� CAN
CAN_FrameTypeDef CAN_ExtractFrame(uint8_t buffer_number);  // ���������� ��������� ����� �� ������
void Thread_Transmitter(void *argument);                   // ����� ����������� CAN
void Thread_Receiver(void *argument);                      // ����� ��������� CAN

#endif
