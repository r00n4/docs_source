/*******************************************************************************
 * @File: dac.c
 * @Author: Milandr, L.
 * @Project: Sample 5.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 30.03.2017
 * @Purpose: ���������� �����-���������� ����������������
 *******************************************************************************/

// ����������� ������������� �����
#include "dac.h"

// ������ �������� �������
volatile uint16_t signal[SIGNAL_SAMPLE_SIZE];

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
  MDR_DAC->CFG = (0 << DAC_CFG_M_REF1_Pos)    // ����� ��������� �������� ���������� (����������)
               | (1 << DAC_CFG_ON_DAC1_Pos);  // ������ ��� (�������)
}

// ��������� ��������������� �������
void GenerateSinus(volatile uint16_t *buffer)
{
  float    omega;  // ������ �������
  uint16_t i;

  // ������� ������������������� ������� �� �������� ������
  omega = 2 * PI / SIGNAL_SAMPLE_SIZE;

  // ���� ������������ �������
  for (i = 0; i < SIGNAL_SAMPLE_SIZE; i++)
    buffer[i] = VLT_TO_DGT(SIGNAL_CONSTANT + SIGNAL_AMPLITUDE * sin(omega * i));
}

// ��������� ������������� �������
void GenerateSaw(volatile uint16_t *buffer)
{
  float    k;
  uint16_t i;

  // ������� ��������� ������� �� �������� ������
  // (��� ��������� ��������� �����������)
  k = SIGNAL_AMPLITUDE / SIGNAL_SAMPLE_SIZE;

  // ���� ������������ �������
  for (i = 0; i < SIGNAL_SAMPLE_SIZE; i++)
     buffer[i] = VLT_TO_DGT(SIGNAL_CONSTANT + k * i);
}

// ����������� ���������� ������� �� �������
void DAC_PrintInfo(void)
{
  // ���������, ������������ �� �������
  char message[LCD_STR_LEN];

  // ������ �������
  snprintf(message, LCD_STR_LEN, "�������:   %d", SIGNAL_SAMPLE_SIZE);
  LCD_PutString(message, 3);

  // ������� �������
  snprintf(message, LCD_STR_LEN, "�������:   %d ��", SIGNAL_FREQUENCY);
  LCD_PutString(message, 4);

  // ���������� ������������ �������
  snprintf(message, LCD_STR_LEN, "�����.:    %.2f �", SIGNAL_CONSTANT);
  LCD_PutString(message, 5);

  // ��������� �������
  snprintf(message, LCD_STR_LEN, "���������: %.2f �", SIGNAL_AMPLITUDE);
  LCD_PutString(message, 6);
}
