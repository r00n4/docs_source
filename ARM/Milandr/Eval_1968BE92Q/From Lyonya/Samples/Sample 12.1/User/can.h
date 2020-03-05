/*******************************************************************************
 * @File: can.h
 * @Author: Milandr, L.
 * @Project: Sample 12.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 31.08.2017
 * @Purpose: Работа с модулем CAN
 *******************************************************************************/

#ifndef CAN_H
#define CAN_H

// Подключение заголовочных файлов модулей проекта
#include "link.h"

// Номера буферов модуля CAN для приема и передачи
#define RX_BUF_NUM 0
#define TX_BUF_NUM 1

// Событие о приеме данных
#define EVENT_CAN_DATA_RECEIVER 0x00000001

// Структура кадра
typedef struct
{
  uint32_t ID;        // Идентификатор
  uint8_t  IDE;       // Формат кадра
  uint8_t  DLC;       // Код длины данных
  uint32_t DATAL;     // Байты данных 1...4
  uint32_t DATAH;     // Байты данных 5...8
} CAN_FrameTypeDef;

// Прототипы функций
void CAN_Reset(MDR_CAN_TypeDef *MDR_CANx);                 // Деинициализация контроллера CAN
void CAN_PortCfg(void);                                    // Конфигурация порта для работы контроллера CAN (CAN1)
void CAN_Init(void);                                       // Инициализация контроллера CAN
CAN_FrameTypeDef CAN_ExtractFrame(uint8_t buffer_number);  // Извлечение принятого кадра из буфера
void Thread_Transmitter(void *argument);                   // Поток передатчика CAN
void Thread_Receiver(void *argument);                      // Поток приемника CAN

#endif
