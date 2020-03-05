/*******************************************************************************
 * @File: tmr.c
 * @Author: Milandr, L.
 * @Project: Sample 7.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 04.04.2017
 * @Purpose: ������������ ���������� ��������
 *******************************************************************************/

// ����������� ������������� �����
#include "tmr.h"

// ����������� ���������� ��������� (� ���������)
int8_t duty_cycle = 50;

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

// ������������ ����� ��� ������ � ��������
void TIMER_PortCfg(void)
{
  // ��������� ������������ ����� F
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTF_Pos);

  // ������������ ����� PF6 ��� ������ � �������� ������ �������
  MDR_PORTF->OE     |=  (1 << 6);   // ����������� ����� (�����)
  MDR_PORTF->FUNC   &= ~(3 << 12);  // ����� ����� �������� FUNC
  MDR_PORTF->FUNC   |=  (2 << 12);  // ������� ����� (��������������)
  MDR_PORTF->ANALOG |=  (1 << 6);   // ����� ������ ����� (��������)
  MDR_PORTF->PULL   &= ~(1 << 6);   // �������� � ���� ������� (���������)
  MDR_PORTF->PULL   &= ~(1 << 22);  // �������� � ����� (���������)
  MDR_PORTF->PD     &= ~(1 << 6);   // ���������� ������ (�������)
  MDR_PORTF->PD     &= ~(1 << 22);  // ������� ������ (��������)
  MDR_PORTF->PWR    |=  (3 << 12);  // �������� ��������� (�������)
  MDR_PORTF->GFEN   &= ~(1 << 6);   // �������� ������ (��������)
}

// ������������� ������� � ������ �������-���������� ���������
void TIMER_Init(void)
{
  // ��������� ������������ �������
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_TIMER1_Pos);
  MDR_RST_CLK->TIM_CLOCK |= (1 << RST_CLK_TIM_CLOCK_TIM1_CLK_EN_Pos);

  // ��������������� �������
  TIMER_Reset(MDR_TIMER1);

  // ������������ ����� ��� ������ � ��������
  TIMER_PortCfg();

  // ������������ �������� ������� �������
  MDR_TIMER1->PSG = TIMER1_PRESCALER - 1;

  // �������� �������������� ������������ �������
  MDR_TIMER1->ARR = PULSE_PERIOD(PULSE_FREQUENCY);

  // ����� ������������ �������
  MDR_TIMER1->CNTRL = (0 << TIMER_CNTRL_CNT_EN_Pos)             // ������ ������� (���� ��������)
                    | (1 << TIMER_CNTRL_ARRB_EN_Pos)            // ����� ���������� �������� ARR (��� ������������)
                    | (0 << TIMER_CNTRL_DIR_Pos)                // ����������� ����� (������ ����)
                    | (0 << TIMER_CNTRL_FDTS_Pos)               // ������� ������� (�� ������������)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)           // ����� ����� (�������� �������� � ������������� ������������)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);         // �������� ������� ��� ����� (�������� ����� �������� ���������)

  // ����� ������������ ������ CH1 �������
  MDR_TIMER1->CH1_CNTRL  = (0 << TIMER_CH_CNTRL_CHFLTR_Pos)     // ���������� �������� ������� (�� ������������)
                         | (0 << TIMER_CH_CNTRL_CHSEL_Pos)      // ������� ��� �������� �������� � ������� CCR (�� ������������)
                         | (0 << TIMER_CH_CNTRL_CHPSC_Pos)      // ������������ ������� �������� ������� (�� ������������)
                         | (0 << TIMER_CH_CNTRL_OCCE_Pos)       // ������������� ������ ETR (�� ������������)
                         | (6 << TIMER_CH_CNTRL_OCCM_Pos)       // ������ ��������� ������� (REF) (CHOUT = {!DIR ��� CNT <  CCR,
                                                                //                                           DIR ��� CCR <= CNT <= ARR})
                         | (0 << TIMER_CH_CNTRL_BRKEN_Pos)      // ����� �� ������ BRK (�� ������������)
                         | (0 << TIMER_CH_CNTRL_ETREN_Pos)      // ����� �� ������ ETR (�� ������������)
                         | (0 << TIMER_CH_CNTRL_CAP_NPWM_Pos);  // ����� ������ ������ (���)

  // ������������ ����� ������ �������
  // ������ �����
  MDR_TIMER1->CH1_CNTRL1 = (1 << TIMER_CH_CNTRL1_SELOE_Pos)     // ���������� ��������� ��������� ������� �� ������ ����� (���������)
                         | (2 << TIMER_CH_CNTRL1_SELO_Pos)      // ���������� ������� ������� ����� (REF)
                         | (0 << TIMER_CH_CNTRL1_INV_Pos)       // �������� ������� ����� (�����������)

  // ...��������� �����
                         | (0 << TIMER_CH_CNTRL1_NSELOE_Pos)    // ���������� ��������� ��������� ������� �� ��������� ����� (���������)
                         | (0 << TIMER_CH_CNTRL1_NSELO_Pos)     // ���������� ������� ��������� ����� (���������)
                         | (0 << TIMER_CH_CNTRL1_NINV_Pos);     // �������� ��������� ����� (�����������)

  // ������������ �������� ���������
  MDR_TIMER1->CH1_CNTRL2 = (0 << TIMER_CH_CNTRL2_CHSEL1_Pos)    // ������� ��� �������� �������� �������� CCR1 (�� ������������)
                         | (0 << TIMER_CH_CNTRL2_CCR1_EN_Pos)   // ������������� �������� CCR1 (�� ������������)
                         | (1 << TIMER_CH_CNTRL2_CCRRLD_Pos);   // ���������� ��������� CCR � CCR1 (��� ������������)

  // ������� ��������� �������� ������������ ���������� ���������
  PWM_SetDutyCycle(duty_cycle);

  // ������ �������
  MDR_TIMER1->CNTRL |= (1 << TIMER_CNTRL_CNT_EN_Pos);
}

// ������� ���������� ���������
void PWM_SetDutyCycle(uint8_t duty_cycle)
{
  // ���������, ������������ �� �������
  char message[LCD_STR_LEN];

  // ���������� ������ ��������� (� �������� �������),
  // � ������ ������������ �������� � ������� ��������� CCR
  MDR_TIMER1->CCR1 = PULSE_PERIOD(PULSE_FREQUENCY) * duty_cycle / 100;

  // ���� ��������� �������� 100-���������� ����������� ����������,
  // �� ������� �������� CCR �� ������� ������ �������,
  // ����� ��������� ����������� ������� �������
  if (duty_cycle == 100)
    MDR_TIMER1->CCR1 = PULSE_PERIOD(PULSE_FREQUENCY) + 1;

  // ����������� ��������� ����������� ���������� �� �������
  snprintf(message, LCD_STR_LEN, "       D = %u%%", duty_cycle);
  LCD_PutString(message, 4);
}
