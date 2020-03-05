/*******************************************************************************
 * @File: spi.h
 * @Author: Milandr, L.
 * @Project: Sample 10.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 19.05.2017
 * @Purpose: ���������� ���������� SPI
 *******************************************************************************/

#ifndef SPI_H
#define SPI_H

// ����������� ������������ ������ ������� �������
#include "link.h"

// ��������� �������� �������� �� ���������� SPI
#define SPI_SPEED 1000000

// ��������� ������ � ������ SS
#define SPI_LowerSS  MDR_PORTF->RXTX &= ~(1 << 2)
#define SPI_RaiseSS  MDR_PORTF->RXTX |= (1 << 2)

// ��������� �������
void SPI_PortCfg(void);                                                                                  // ������������ ����� ��� ������ ������ SSP1
void SPI_Init(void);                                                                                     // ������������� ���������� SPI
void SPI_SimpleTransmit(MDR_SSP_TypeDef *MDR_SSPx, uint8_t data);                                        // ���������� �������� ������ �� ���������� SPI
void SPI_TransmitData(MDR_SSP_TypeDef *MDR_SSPx, uint8_t address, uint8_t data);                         // �������� ������ �� ���������� SPI
uint8_t SPI_ReceiveData(MDR_SSP_TypeDef *MDR_SSPx, uint8_t address);                                     // ����� ������ �� ���������� SPI
void SPI_ReceiveDataBurst(MDR_SSP_TypeDef *MDR_SSPx, uint8_t address, uint8_t amount, uint8_t *buffer);  // ���������������� ����� ���������� ������� ������

#endif
