 /*******************************************************************************
 * @File: tmr.c
 * @Author: Milandr, L.
 * @Project: Sample 3.2
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 15.03.2019
 * @Purpose: ������������ ���������� ��������/���������
 *******************************************************************************/

// ����������� ������������� �����
#include "tmr.h"

// ��������������� �������
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx)
{
  MDR_TIMERx->CNT          = 0;
  MDR_TIMERx->PSG          = 0;
  MDR_TIMERx->ARR          = 0;
  MDR_TIMERx->CNTRL        = 0;
  MDR_TIMERx->CCR1         = 0;
  MDR_TIMERx->CCR2         = 0;
  MDR_TIMERx->CCR3         = 0;
  MDR_TIMERx->CCR4         = 0;
  MDR_TIMERx->CCR11        = 0;
  MDR_TIMERx->CCR21        = 0;
  MDR_TIMERx->CCR31        = 0;
  MDR_TIMERx->CCR41        = 0;
  MDR_TIMERx->CH1_CNTRL    = 0;
  MDR_TIMERx->CH2_CNTRL    = 0;
  MDR_TIMERx->CH3_CNTRL    = 0;
  MDR_TIMERx->CH4_CNTRL    = 0;
  MDR_TIMERx->CH1_CNTRL1   = 0;
  MDR_TIMERx->CH2_CNTRL1   = 0;
  MDR_TIMERx->CH3_CNTRL1   = 0;
  MDR_TIMERx->CH4_CNTRL1   = 0;
  MDR_TIMERx->CH1_CNTRL2   = 0;
  MDR_TIMERx->CH2_CNTRL2   = 0;
  MDR_TIMERx->CH3_CNTRL2   = 0;
  MDR_TIMERx->CH4_CNTRL2   = 0;
  MDR_TIMERx->CH1_DTG      = 0;
  MDR_TIMERx->CH2_DTG      = 0;
  MDR_TIMERx->CH3_DTG      = 0;
  MDR_TIMERx->CH4_DTG      = 0;
  MDR_TIMERx->BRKETR_CNTRL = 0;
  MDR_TIMERx->STATUS       = 0;
  MDR_TIMERx->IE           = 0;
  MDR_TIMERx->DMA_RE       = 0;
}

// ������������� ������� (TIMER1)
void TIMER_Init(void)
{
  // ��������� ������������ �������
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_TIMER1_Pos);
  MDR_RST_CLK->TIM_CLOCK |= (1 << RST_CLK_TIM_CLOCK_TIM1_CLK_EN_Pos);

  // ��������������� �������
  TIMER_Reset(MDR_TIMER1);

  // ������������ �������� ������� �������
  MDR_TIMER1->PSG = TIMER1_PRESCALER - 1;

  // �������� �������������� ������������ �������
  MDR_TIMER1->ARR = 0xFFFF;

  // ����� ������������ �������
  MDR_TIMER1->CNTRL = (1 << TIMER_CNTRL_CNT_EN_Pos)      // ������ ������� (�������)
                    | (0 << TIMER_CNTRL_ARRB_EN_Pos)     // ����� ���������� �������� ARR (����������)
                    | (0 << TIMER_CNTRL_DIR_Pos)         // ����������� ����� (������ ����)
                    | (0 << TIMER_CNTRL_FDTS_Pos)        // ������� ������� (�� ������������)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)    // ����� ����� (�������� �������� � ������������� ������������)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);  // �������� ������� ��� ����� (�������� ����� �������� ���������)
}

// ����� ����������� �������
void Thread_TimeCounter(void *argument)
{
  uint16_t count;             // �������� ��������
  float time;                 // ����� ������ �������
  char message[LCD_STR_LEN];  // ���������, ������������ �� �������

  // �������� ����
  while (1)
  {
    // ���������� �������� ��������
    count = MDR_TIMER1->CNT;

    // �������������� �������� ������� �� �����
    time = CNT_TO_TIME(count);

    // ����������� ��������� ���������� �� �������
    snprintf(message, LCD_STR_LEN, "      T = %.2f s", time);
    LCD_PutString(message, 3);

    snprintf(message, LCD_STR_LEN, "      C = %u", count);
    LCD_PutString(message, 5);

    // �������� ����� ������� ��������� ��������
    osDelay(200);
  }
}
