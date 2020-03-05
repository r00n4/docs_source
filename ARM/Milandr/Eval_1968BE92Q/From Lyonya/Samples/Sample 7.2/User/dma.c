/*******************************************************************************
 * @File: dma.c
 * @Author: Milandr, L.
 * @Project: Sample 7.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: ������������ ������� ������� � ������
 *******************************************************************************/

// ����������� ������������� �����
#include "dma.h"

// ��������� ����������� ������ ��� ����������� DMA
// 32 ������; �������� � �������������� ���������; ����� - 64 ��������
// ��������� ������� �� ������� 32-������ �����; ����� - 1024 �����
volatile DMA_ControlDataTypeDef DMA_ControlData[64] __attribute__ ((aligned (1024)));

// ��������������� ����������� DMA
void DMA_Reset(void)
{
  MDR_DMA->CFG               = 0;
  MDR_DMA->CTRL_BASE_PTR     = 0;
  MDR_DMA->CHNL_SW_REQUEST   = 0;
  MDR_DMA->CHNL_USEBURST_CLR = 0xFFFFFFFF;
  MDR_DMA->CHNL_REQ_MASK_CLR = 0xFFFFFFFF;
  MDR_DMA->CHNL_ENABLE_CLR   = 0xFFFFFFFF;
  MDR_DMA->CHNL_PRI_ALT_CLR  = 0xFFFFFFFF;
  MDR_DMA->CHNL_PRIORITY_CLR = 0xFFFFFFFF;
  MDR_DMA->ERR_CLR           = 1;
}

// ������������� ����������� DMA
void DMA_Init(void)
{
  // ��������� ������������ ����������� DMA
  // ��� ���������� ������ ����������� DMA ����� ���������� �������� ������������ ������������ SSP1 � SSP2
  // (���� ����� �� �������, �� ��������� ����� ��������� ���������� �� ����������� DMA)
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_DMA_Pos)    // ���������� DMA
                          | (1 << RST_CLK_PCLK_SSP1_Pos)   // ���������� SSP1
                          | (1 << RST_CLK_PCLK_SSP2_Pos);  // ���������� SSP2

  // ����� ���������� �� ����������� DMA
  NVIC_ClearPendingIRQ(DMA_IRQn);

  // ��������������� ����������� DMA
  DMA_Reset();

  // ������������ ����������� ������ ������ 8 (ADC1)
  DMA_ControlData[8].DMA_Control = (1 << DMA_CHANNEL_CFG_CYCLE_CTRL_Pos)                     // ����� ������ ����������� DMA (��������)
                                 | (0 << DMA_CHANNEL_CFG_NEXT_USEBURST_Pos)                  // ����� �������� �������� (��������)
                                 | ((ADC_SAMPLE_SIZE - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos)  // ���������� ������� � ����� (�� ������� �������)
                                 | (0 << DMA_CHANNEL_CFG_R_POWER_Pos)                        // ���������� ������� �� ���������� ��������� (1)
                                 | (1 << DMA_CHANNEL_CFG_SRC_PROT_CTRL_Pos)                  // ����� ������ ��������� (�����������������)
                                 | (1 << DMA_CHANNEL_CFG_DST_PROT_CTRL_Pos)                  // ����� ������ ��������� (�����������������)
                                 | (1 << DMA_CHANNEL_CFG_SRC_SIZE_Pos)                       // ����������� ������ ��������� (2 �����)
                                 | (3 << DMA_CHANNEL_CFG_SRC_INC_Pos)                        // ��������� ������ ��������� ������ (�����������)
                                 | (1 << DMA_CHANNEL_CFG_DST_SIZE_Pos)                       // ����������� ������ ��������� (2 �����)
                                 | (1U << DMA_CHANNEL_CFG_DST_INC_Pos);                      // ��������� ������ ��������� ������ (2 �����)

  // ������������ ��������� ����� ����� ���������
  DMA_ControlData[8].DMA_SourceEndPointer = (uint32_t)&MDR_ADC->ADC1_RESULT;

  // ������������ ��������� ����� ����� ���������
  DMA_ControlData[8].DMA_DestinationEndPointer = (uint32_t)&sample + ((ADC_SAMPLE_SIZE - 1) << 1);

  // ������� �������� ������ ����������� ������
  MDR_DMA->CTRL_BASE_PTR = (uint32_t)&DMA_ControlData;

  // ������������ ����������� DMA
  MDR_DMA->CFG = (1 << DMA_CFG_MASTER_ENABLE_Pos)    // ������ ����������� DMA (�������)
               | (1 << DMA_CFG_CHNL_PROT_CTRL_Pos);  // ����� ������ ���� (�����������������)

  // ���������� ������ ������ 8
  MDR_DMA->CHNL_ENABLE_SET = (1 << 8);

  // ������������� *��������* ��������� ���������� ��� ������ 8
  MDR_DMA->CHNL_PRI_ALT_CLR = (1 << 8);

  // ��������� ������ 8 (�������)
  MDR_DMA->CHNL_PRIORITY_CLR = (1 << 8);

  // ���������� ���������� ���������� �� ����������� DMA
  NVIC_SetPriority(DMA_IRQn, 1);

  // ���������� ��������� ���������� �� ����������� DMA
  NVIC_EnableIRQ(DMA_IRQn); 
}

// ���������� ����������� ���������� �� DMA
void DMA_IRQHandler(void)
{
  // ���������� ���
  MDR_ADC->ADC1_CFG &= ~(1 << ADC1_CFG_REG_ADON_Pos);

  // ��������� ����� ���������� ����� �������
  osEventFlagsSet(EventId_ADC, EVENT_ADC_EOC);

  // ������������ ������ ����� �������
  // ����� ����������� ����� ��������� ����������� ������
  DMA_ControlData[8].DMA_Control |= (1 << DMA_CHANNEL_CFG_CYCLE_CTRL_Pos)                     // ����� ������ (��������)
                                 | ((ADC_SAMPLE_SIZE - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos);  // ���������� ������� � �����

  // ���������� ������ ������ 8
  MDR_DMA->CHNL_ENABLE_SET = (1 << 8);
}
