/*******************************************************************************
 * @File: uart.c
 * @Author: Milandr, L.
 * @Project: Sample 9.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 02.03.2017
 * @Purpose: ������ � ������������ UART
 *******************************************************************************/
 
// ����������� ������������� �����
#include "uart.h"

// ��������������� ����������� UART
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx)
{
  MDR_UARTx->RSR_ECR = 0;
  MDR_UARTx->FR      = UART_FR_RXFE   // ����� ���� ������, ����� ������
                     | UART_FR_TXFE;  // ���������� ������ � ��������� � �����������
  MDR_UARTx->ILPR    = 0;
  MDR_UARTx->IBRD    = 0;
  MDR_UARTx->FBRD    = 0;
  MDR_UARTx->LCR_H   = 0;
  MDR_UARTx->CR      = 0;
  MDR_UARTx->IFLS    = 0;
  MDR_UARTx->IMSC    = 0;
  MDR_UARTx->DMACR   = 0;
}

// ������������ ����� ��� ������ ����������� UART (UART2)
void UART_PortCfg(void)
{
  // ������������ ����� PF0 (RX) � PF1 (TX)
  MDR_PORTF->OE     &= ~((1 << 0));               // ����������� ������ (PF0 - ����)
  MDR_PORTF->OE     |=  ((1 << 1));               // ����������� ������ (PF1 - �����)
  MDR_PORTF->PULL   &= ~((1 << 0)  | (1 << 1));   // �������� � ����� (���������)
  MDR_PORTF->PULL   &= ~((1 << 16) | (1 << 17));  // �������� � ������� (���������)
  MDR_PORTF->ANALOG |=  ((1 << 0)  | (1 << 1));   // ����� ������ ����� (��������)
  MDR_PORTF->FUNC   |=  ((3 << 0)  | (3 << 2));   // ������� ����� (����������������)
  MDR_PORTF->PD     &= ~((1 << 0)  | (1 << 1));   // ���������� ������� (�� ������������)
  MDR_PORTF->PD     &= ~((1 << 16) | (1 << 17));  // �������� ������ (�� ������������)
  MDR_PORTF->PWR    |=  ((3 << 0)  | (3 << 2));   // �������� ������� (�������)
  MDR_PORTF->GFEN   &= ~((1 << 0)  | (1 << 1));   // �������� ������� (�� ������������)
}

// ������������� ������ UART (UART2)
void UART_Init(void)
{
  // ��������� ������������ ����� F
  MDR_RST_CLK->PER_CLOCK |= (1 << RST_CLK_PCLK_PORTF_Pos);

  // ������������ ����� ��� ������ ����������� UART
  UART_PortCfg();

  // ��������� ������������ ����������� UART2
  MDR_RST_CLK->PER_CLOCK  |= (1 << RST_CLK_PCLK_UART2_Pos);
  MDR_RST_CLK->UART_CLOCK |= (1 << RST_CLK_UART_CLOCK_UART2_CLK_EN_Pos);

  // ��������������� ����������� UART
  UART_Reset(MDR_UART2);

  // ������� �������� �������� ������
  // ���������� ���������� �������� �������� �������� �������
  uint32_t divider  = SystemCoreClock / (UART_SPEED >> 2);

  // ������ � ������� ����� ����� ��������
  MDR_UART2->IBRD = divider >> 6;

  // ������ � ������� ������� ����� ��������
  MDR_UART2->FBRD = divider & 0x3F;

  // ������������ ������� ������
  MDR_UART2->LCR_H = (1 << UART_LCR_H_PEN_Pos)   // ������������� ������������ ���� (��������)
                   | (1 << UART_LCR_H_EPS_Pos)   // �������� �� �������� ��� ���������� (��������)
                   | (0 << UART_LCR_H_STP2_Pos)  // ���������� STOP-����� (����)
                   | (0 << UART_LCR_H_FEN_Pos)   // ������������� ������ FIFO (�� ������������)
                   | (3 << UART_LCR_H_WLEN_Pos)  // ���������� ����� ������ (8)
                   | (0 << UART_LCR_H_SPS_Pos);  // �������� �������� ������������ ���� (���������)

  // ������������ ����������� UART
  MDR_UART2->CR = (1 << UART_CR_UARTEN_Pos)  // ������ ����������� UART (�������)
                | (0 << UART_CR_SIREN_Pos)   // ������ ��-������ (��������)
                | (0 << UART_CR_SIRLP_Pos)   // ������������ ��������� ��� ��-�������� (�� ������������)
                | (0 << UART_CR_LBE_Pos)     // ����� ���������������� (��������)
                | (1 << UART_CR_TXE_Pos)     // ���������� �������� (���������)
                | (1 << UART_CR_RXE_Pos)     // ���������� ������ (��������)
                | (0 << UART_CR_DTR_Pos)     // �������� ������� �� ����� DTR (�� ������������)
                | (0 << UART_CR_RTS_Pos)     // �������� ������� �� ����� RTS (�� ������������)
                | (0 << UART_CR_OUT1_Pos)    // �������� ������� �� ����� OUT1 (�� ������������)
                | (0 << UART_CR_OUT2_Pos)    // �������� ������� �� ����� OUT2 (�� ������������)
                | (0 << UART_CR_RTSEN_Pos)   // ������������� ����� RTS (�� ������������)
                | (0 << UART_CR_CTSEN_Pos);  // ������������� ����� CTS (�� ������������)

  // ��������� �������� �� ��������� ���������� �� ����������� UART
  MDR_UART2->IMSC = (1 << UART_IMSC_RXIM_Pos);  // ���������� �� ���������

  // ���������� ���������� ���������� �� ����������� UART
  NVIC_SetPriority(UART2_IRQn, 1);

  // ���������� ��������� ���������� �� ����������� UART
  NVIC_EnableIRQ(UART2_IRQn);
}

// ����������� �������� � ����������� ���������
void LCD_PrintSymbol(char symbol)
{
  static int8_t x, y;  // ���������� �������

  // ��������� ������
  switch(symbol)
  {
    // ��������� �������� �������� (Backspace / Delete)
    case 0x08:
    case 0x7F:
      LCD_PutSymbol(' ', x, y);
      x--;
      if (x < 0)
      {
        x = 20;
        y--;
        if (y < 0)
          x = y = 0;
      }
      LCD_PutSymbol('_', x, y);
      break;

    // ��������� �������� �������� �� ����� ������ (Enter)
    case 0x0D:
      LCD_PutSymbol(' ', x, y);
      x = 0;
      y++;
      if (y > 7)
        y = 0;
      LCD_PutSymbol('_', x, y);
      break;

    // ��������� �������� ������� ������� (Escape)
    case 0x1B:
      LCD_ClearScreen();
      LCD_PutSymbol('_', 0, 0);
      x = y = 0;
      break;

    // ��������� ��������
    default:
    LCD_PutSymbol(symbol, x, y);
    x++;
    if (x > 20)
    {
      x = 0;
      y++;
      if (y > 7)
        y = 0;
    }
    LCD_PutSymbol('_', x, y);
    break;
  }
}

// ����� �������� �������� � ���������� �� �� ������� ���������� �����
void Thread_Typewriter(void *argument)
{
  // ���������� ��� �������� ���������� ������
  uint8_t data;

  // ����������� ��������� �� �������
  LCD_PutSymbol('_', 0, 0);

  // �������� ����
  while(1)
  {
    // �������� ������ ������
    osEventFlagsWait(EventId_UART, EVENT_DATA_RECEIVED, osFlagsWaitAny, osWaitForever);

    // ���������� �������� ������
    data = MDR_UART2->DR;

    // ����������� ��������� ������� �� �������
    LCD_PrintSymbol(data);

    // �������� ������ � �������� �����������
    MDR_UART2->DR = data;
  }
}

// ���������� ���������� �� ����������� UART2
void UART2_IRQHandler(void)
{
    // ����� ������� �� ��������� ����������
    MDR_UART2->ICR |= (1 << UART_ICR_RXIC_Pos);

    // ��������� ����� ���������� ������ ������
    osEventFlagsSet(EventId_UART, EVENT_DATA_RECEIVED);
}
