/*******************************************************************************
 * @File: spi.h
 * @Author: Milandr, L.
 * @Project: Sample 10.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 19.05.2017
 * @Purpose: Реализация интерфейса SPI
 *******************************************************************************/

#ifndef SPI_H
#define SPI_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Требуемая скорость передачи по интерфейсу SPI
#define SPI_SPEED 1000000

// Директивы работы с линией SS
#define SPI_LowerSS  MDR_PORTF->RXTX &= ~(1 << 2)
#define SPI_RaiseSS  MDR_PORTF->RXTX |= (1 << 2)

// Прототипы функций
void SPI_PortCfg(void);                                                                                  // Конфигурация порта для работы модуля SSP1
void SPI_Init(void);                                                                                     // Инициализация интерфейса SPI
void SPI_SimpleTransmit(MDR_SSP_TypeDef *MDR_SSPx, uint8_t data);                                        // Упрощенная передача данных по интерфейсу SPI
void SPI_TransmitData(MDR_SSP_TypeDef *MDR_SSPx, uint8_t address, uint8_t data);                         // Передача данных по интерфейсу SPI
uint8_t SPI_ReceiveData(MDR_SSP_TypeDef *MDR_SSPx, uint8_t address);                                     // Прием данных по интерфейсу SPI
void SPI_ReceiveDataBurst(MDR_SSP_TypeDef *MDR_SSPx, uint8_t address, uint8_t amount, uint8_t *buffer);  // Последовательный прием нескольких пакетов данных

#endif
