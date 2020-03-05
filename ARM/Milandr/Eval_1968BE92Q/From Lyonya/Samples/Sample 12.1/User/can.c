/*******************************************************************************
 * @File: can.c
 * @Author: Milandr, L.
 * @Project: Sample 12.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.08.2017
 * @Purpose: ������ � ������� CAN
 *******************************************************************************/
 
// ����������� ������������� �����
#include "can.h"

// ��������������� ����������� CAN
void CAN_Reset(MDR_CAN_TypeDef *MDR_CANx)
{
  uint32_t i;

  MDR_CANx->CONTROL = 0;
  MDR_CANx->STATUS  = 0;
  MDR_CANx->BITTMNG = 0;
  MDR_CANx->INT_EN  = 0;
  MDR_CANx->OVER    = 0;
  MDR_CANx->INT_RX  = 0;
  MDR_CANx->INT_TX  = 0;

  // ����� ��������� ���������� ��������
  for (i = 0; i < 32; i++)
    MDR_CANx->BUF_CON[i] = 0;

  // ����� ��������
  for (i = 0; i < 32; i++)
  {
    MDR_CANx->CAN_BUF_FILTER[i].FILTER = 0;
    MDR_CANx->CAN_BUF_FILTER[i].MASK   = 0;
  }
}

// ������������ ����� ��� ������ ����������� CAN1
void CAN_PortCfg(void)
{
  // ��������� ������������ ����� A
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTA_Pos);

  // ������������ ����� PA6 (TX) � PA7 (RX)
  MDR_PORTA->OE     |=  ((1 << 6));               // ����������� �������� ������ (PA6 - �����)
  MDR_PORTA->OE     &= ~((1 << 7));               // ����������� �������� ������ (PA7 - ����)
  MDR_PORTA->FUNC   &= ~((3 << 12) | (3 << 14));  // ����� ����� �������� FUNC
  MDR_PORTA->FUNC   |=  ((2 << 12) | (2 << 14));  // ������� ����� (��������������)
  MDR_PORTA->ANALOG |=  ((1 << 6)  | (1 << 7));   // ����� ������ ����� (��������)
  MDR_PORTA->PULL   &= ~((1 << 6)  | (1 << 7));   // ��������� �������� � ���� ������� (���������)
  MDR_PORTA->PULL   &= ~((1 << 22) | (1 << 23));  // ��������� �������� � ����� (���������)
  MDR_PORTA->PD     &= ~((1 << 6)  | (1 << 7));   // ���������� ������� (�������)
  MDR_PORTA->PD     &= ~((1 << 22) | (1 << 23));  // �������� ������ (���������)
  MDR_PORTA->PWR    |=  ((3 << 12) | (3 << 14));  // ������������ ������� ��������� (������)
  MDR_PORTA->GFEN   &= ~((1 << 6)  | (1 << 7));   // �������� ������� (���������)
}

// ������������� ����������� CAN (CAN1)
void CAN_Init(void)
{
  // ������������ ����� ��� ������ ����������� CAN
  CAN_PortCfg();

  // ��������� ������������ ����������� CAN
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_CAN1_Pos);
  MDR_RST_CLK->CAN_CLOCK |= (1 << RST_CLK_CAN_CLOCK_CAN1_CLK_EN_Pos);

  // ��������������� ����������� CAN
  CAN_Reset(MDR_CAN1);

  // ������������ ����������� CAN
  MDR_CAN1->CONTROL = (1 << 0)   // ������ ����������� (�������)
                    | (0 << 1)   // ����� "������ ������" (��������)
                    | (1 << 2)   // ����� ���������������� (�������)
                    | (1 << 3)   // �����. ������ ����������� ������ (��������)
                    | (1 << 4);  // ����� ����������� ������ (�������)

  // ������������ �������� ���������
  MDR_CAN1->BITTMNG = (15 << 0)   // �������� �������� ������� (16 � 1)
                    | (1 << 16)   // ������������ �������� ��������������� (2 TQ)
                    | (3 << 19)   // ������������ ������� �������� �������� (4 TQ)
                    | (2 << 22)   // ������������ ������� �������� �������� (3 TQ)
                    | (1 << 25)   // ���������� �������� ���������� ��������� (2 TQ)
                    | (0 << 27);  // ����� ������� (�����������)

  // ������������ �������
  // ����� ������
  MDR_CAN1->BUF_CON[RX_BUF_NUM] = (1 << 0)   // ������ ������ (�������)
                                | (1 << 1)   // ����� ������ ������ (�����)
                                | (0 << 2)   // ���������� ���������� ����� (���������)
                                | (0 << 3)   // ���������� ������ �� ��������� ������ (�� ������������)
                                | (1 << 4);  // ��������� ������ (�������)

  // ����� ��������
  MDR_CAN1->BUF_CON[TX_BUF_NUM] = (1 << 0)   // ������ ������ (�������)
                                | (0 << 1)   // ����� ������ ������ (��������)
                                | (0 << 2)   // ���������� ���������� ����� (�� ������������)
                                | (0 << 3)   // ���������� ������ �� ��������� ������ (��������)
                                | (1 << 4);  // ��������� ������ (�������)

  // ��������� �������� �� ��������� ���������� �� ����������� CAN
  MDR_CAN1->INT_EN = (1 << 0)   // ����� ���� ��������
                   | (1 << 1);  // ���������� ��� ������ �����

  // ��������� �������� �� ��������� ���������� ��� ������ ����� � ������������ �����
  MDR_CAN1->INT_RX = (1 << RX_BUF_NUM);

  // ���������� ���������� ���������� �� ����������� CAN
  NVIC_SetPriority(CAN1_IRQn, 1);

  // ���������� ��������� ���������� �� ����������� CAN
  NVIC_EnableIRQ(CAN1_IRQn);
}

// ���������� ��������� ����� �� ������
CAN_FrameTypeDef CAN_ExtractFrame(uint8_t buffer_number)
{
  CAN_FrameTypeDef frame;

  frame.ID    = MDR_CAN1->CAN_BUF[buffer_number].ID;
  frame.IDE   = MDR_CAN1->CAN_BUF[buffer_number].DLC >> 12 & 0x1;
  frame.DLC   = MDR_CAN1->CAN_BUF[buffer_number].DLC & 0xF;
  frame.DATAL = MDR_CAN1->CAN_BUF[buffer_number].DATAL;
  frame.DATAH = MDR_CAN1->CAN_BUF[buffer_number].DATAH;

  return frame;
}

// ����� ����������� CAN
void Thread_Transmitter(void *argument)
{
  // ��������� �������� ��������������
  uint32_t id = 234;

  // �������� ����
  while (1)
  {
    // �������� ������� ������ SEL
    if ((MDR_PORTC->RXTX & (1 << 2)) == 0)
    {
      // ������������ �����
      MDR_CAN1->CAN_BUF[TX_BUF_NUM].ID    = id;          // ������������� �����
      MDR_CAN1->CAN_BUF[TX_BUF_NUM].DLC   = (1 << 0)     // ���� DLC (1 ����)
                                          | (0 << 8)     // ��� RTR (��� �������)
                                          | (1 << 9)     // ��� R1 (������ ������ ���� ����� 1)
                                          | (0 << 10)    // ��� R0 (������ ������ ���� ����� 0)
                                          | (1 << 11)    // ��� SSR (������ ������ ���� ����� 1)
                                          | (1 << 12);   // ��� IDE (����������� ������)
      MDR_CAN1->CAN_BUF[TX_BUF_NUM].DATAL = 0x000000AA;  // ����� ������ 1...4
      MDR_CAN1->CAN_BUF[TX_BUF_NUM].DATAH = 0x00000000;  // ����� ������ 5...8

      // ������ �� �������� ��������������� �����
      MDR_CAN1->BUF_CON[TX_BUF_NUM] |= (1 << 5);

      // �������� ���������� ������
      while ((MDR_PORTC->RXTX & (1 << 2)) == 0)
        osDelay(25);

      // ��������� ��������������
      id++;
    }

    // �������� ����� ������� ��������� ��������
    osDelay(25);
  }
}

// ����� ��������� CAN
void Thread_Receiver(void *argument)
{
  CAN_FrameTypeDef rx_frame;  // ��������� ������������ �����
  char message[LCD_STR_LEN];  // ���������, ������������ �� �������

  // ����� �� ������� ��������� �� ��������
  LCD_PutString("   �������� ����� ", 4);

  // �������� ����
  while (1)
  {
    // �������� ��������� � ��������� ������
    osMessageQueueGet(MsgQueId_CAN, &rx_frame, NULL, osWaitForever);

    // ����������� ���������� ����� �� �������
    sprintf(message, "ID    = %d", rx_frame.ID);
    LCD_PutString(message, 3);
    sprintf(message, "IDE   = %d", rx_frame.IDE);
    LCD_PutString(message, 4);
    sprintf(message, "DLC   = %d", rx_frame.DLC);
    LCD_PutString(message, 5);
    sprintf(message, "DATAL = %d", rx_frame.DATAL);
    LCD_PutString(message, 6);
    sprintf(message, "DATAH = %d", rx_frame.DATAH);
    LCD_PutString(message, 7);
  }
}

// ���������� ���������� �� ����������� CAN1
void CAN1_IRQHandler(void)
{
  // ��������� ������������ �����
  CAN_FrameTypeDef rx_frame;

  // ���������� ������ �� ������
  rx_frame = CAN_ExtractFrame(RX_BUF_NUM);

  // �������� ������
  osMessageQueuePut(MsgQueId_CAN, &rx_frame, osPriorityNormal, 0);

  // ����� ����� ���������� ��� ������ �����
  MDR_CAN1->BUF_CON[RX_BUF_NUM] &= ~(1 << 6);
}

void SomeFunc(uint8_t sum)
{
  uint8_t mass[sum];
}