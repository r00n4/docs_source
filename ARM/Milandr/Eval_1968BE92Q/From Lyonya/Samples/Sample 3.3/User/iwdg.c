/*******************************************************************************
 * @File: iwdg.c
 * @Author: Milandr, L.
 * @Project: Sample 3.3
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 12.07.2017
 * @Purpose: ������������ ����������� �������
 *******************************************************************************/

// ����������� ������������� �����
#include "iwdg.h"

// ������������� ����������� ������� IWDG
void IWDG_Init(void)
{
  // ��������� ���������� LSI
  MDR_BKP->REG_0F |= (1 << BKP_REG_0F_LSI_ON_Pos);

  // �������� ����� ���������� LSI � ������� �����
  while ((MDR_BKP->REG_0F & (1 << BKP_REG_0F_LSI_RDY_Pos)) == 0);

  // ���������� �������� ������� ���������� LSI
  MDR_BKP->REG_0F &= ~BKP_REG_0F_LSI_TRIM_Msk;
  MDR_BKP->REG_0F |= (11 << BKP_REG_0F_LSI_TRIM_Pos);

  // ��������� ������������ ����������� �������
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_IWDT_Pos);

  // ���������� ������ ������ � �������� PR � RLR
  MDR_IWDG->KR = 0x5555;

  // ��������� �������� ������� ����������� �������
  while ((MDR_IWDG->SR & IWDG_SR_PVU) != 0);  // �������� ����� ���������� �������� �������
  MDR_IWDG->PR = 4;                           // ������� �������� (40 ��� / 64)

  // ��������� �������� ������������ ����������� �������
  while ((MDR_IWDG->SR & IWDG_SR_RVU) != 0);  // �������� ����� ���������� �������� ������������
  MDR_IWDG->RLR = 3125;                       // ������� �������� ������������

  // ������ ����������� �������
  MDR_IWDG->KR = 0xCCCC;
}

// ����� ������ �� ���������� ��������
void Thread_Watchdog(void *argument)
{
  // ��������� ������������ ����������� �������
  MDR_IWDG->KR = 0xAAAA;

  // ��������� ����������� ������� ������ �������
  TimerCallback_Counter(NULL);

  // ������ ������������ ������� ��� ������� ������� ������ �������
  osTimerStart(TimerId_Counter, 1000);

  // �������� ����
  while (1)
  {
    // ������ �������, ���������� ��������� � ���������������
    osTimerStart(TimerId_Warning, 2000);

    // �������� ������� ������ SEL
    while ((MDR_PORTC->RXTX & (1 << 2)) != 0)
      osDelay(50);

    // ������������ ����������� �������
    MDR_IWDG->KR = 0xAAAA;

    // ��������� ������������ �������, ���������� ��������� � ���������������
    osTimerStop(TimerId_Warning);

    // �������� ��������� � ���������������
    LCD_ClearString(5);

    // �������� ����� ������� ��������� ��������
    osDelay(50);
  }
}

// ��������� ������� �������, ���������� ��������
void TimerCallback_Counter(void *argument)
{
  static uint32_t t;          // ������� �������
  char message[LCD_STR_LEN];  // ���������, ������������ �� �������

  // ����������� �������� ������� ������ ������� �� �������
  snprintf(message, LCD_STR_LEN, "   ������... %03u", t);
  LCD_PutString(message, 3);

  // ���������� �������� �������
  t++;
}

// ��������� ��������������, ���������� ��������
void TimerCallback_Warning(void *argument)
{
  // ����������� ��������� � ���������������
  LCD_PutString("     ��������!", 5);
}
