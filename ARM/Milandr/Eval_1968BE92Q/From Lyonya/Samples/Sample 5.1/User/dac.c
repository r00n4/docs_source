/*******************************************************************************
 * @File: dac.c
 * @Author: Milandr, L.
 * @Project: Sample 5.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: ���������� �����-���������� ����������������
 *******************************************************************************/

// ����������� ������������� �����
#include "dac.h"

// ��������������� �����-����������� ���������������
void DAC_Reset(void)
{
  MDR_DAC->DAC2_DATA = 0;
  MDR_DAC->CFG       = 0;
}

// ������������ ����� � �������� ������ ���
void DAC_PortCfg(void)
{
  // ��������� ������������ ����� E
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTE_Pos);

  // ������������ ����� PE0 � �������� ������ ���
  MDR_PORTE->OE     &= ~(1 << 0);
  MDR_PORTE->FUNC   &= ~(3 << 0);
  MDR_PORTE->ANALOG &= ~(1 << 0);   // ����� ������ ����� (����������)
  MDR_PORTE->PULL   &= ~(1 << 0);   // �������� � ����� (���������)
  MDR_PORTE->PULL   &= ~(1 << 16);  // �������� � ������� (���������)
  MDR_PORTE->PD     &= ~(1 << 0);
  MDR_PORTE->PD     &= ~(1 << 16);
  MDR_PORTE->PWR    &= ~(3 << 0);
  MDR_PORTE->GFEN   &= ~(1 << 0);
}

// ������������� �����-����������� ���������������
void DAC_Init(void)
{
  // ������������ ����� � �������� ������ ���
  DAC_PortCfg();

  // ��������� ������������ ���
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_DAC_Pos);

  // ��������������� ���
  DAC_Reset();

  // ������������ ���
  MDR_DAC->CFG = (0 << DAC_CFG_SYNC_A_Pos)    //
               | (0 << DAC_CFG_M_REF1_Pos)    // ����� ��������� �������� ���������� (����������)
               | (1 << DAC_CFG_ON_DAC1_Pos);  // ������ DAC2 (�������)

  static const float voltage = 1.20;  // ��������� �������� ���������� (� �������)
  char message[LCD_STR_LEN];          // ���������, ������������ �� �������

  // ������ �����-����������� ��������������
  MDR_DAC->DAC2_DATA = VLT_TO_DGT(voltage);

  // ����������� ���������� �� �������
  snprintf(message, LCD_STR_LEN, "    V = %.2f �", voltage);
  LCD_PutString(message, 3);

  // ����������� ���� ��� �� �������
  snprintf(message, LCD_STR_LEN, "    D = %d", VLT_TO_DGT(voltage));
  LCD_PutString(message, 5);
}
