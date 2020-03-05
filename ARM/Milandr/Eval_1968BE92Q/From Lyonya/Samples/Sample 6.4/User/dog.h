/*******************************************************************************
 * @File: dog.h
 * @Author: Milandr, L.
 * @Project: Sample 6.4
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: ���������� ��������� ����
 *******************************************************************************/

#ifndef DOG_H
#define DOG_H

// ����������� ����������� ���������� ����� ��
#include "stdlib.h"

// ����������� ������������ ������ ������� �������
#include "link.h"

// ����� ������ (� �������������)
#define DOGGIE_GAME_TIME 60001

// �������� ����� �������������� "�������" (� �������������)
#define DOGGIE_MOVE_TIME 1807

// ������� �������� ����
#define FIELD_X_MIN 4
#define FIELD_X_MAX 16
#define FIELD_Y_MIN 2
#define FIELD_Y_MAX 5

// ��������� �������
void DOG_MoveCursor(uint16_t digitX, uint16_t digitY);  // ������������ ��������� �� �������
void Thread_DoggieInit(void *argument);                 // ����� ������������� ����
void TimerCallback_MoveDoggie(void *argument);          // ��������� ������������ "�������", ���������� ��������
void TimerCallback_TimeCounter(void *argument);         // ��������� ������� ������� ����, ���������� ��������
void TimerCallback_GameOver(void *argument);            // ��������� ���������� ����, ���������� ��������

#endif
