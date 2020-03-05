/*******************************************************************************
 * @File: dog.c
 * @Author: Milandr, L.
 * @Project: Sample 6.4
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: ���������� ��������� ����
 *******************************************************************************/

// ����������� ������������� �����
#include "dog.h"

// ���������� ��������� �� �������
uint8_t symbol_x = FIELD_X_MIN;
uint8_t symbol_y = FIELD_Y_MIN;

// ������� ������� �����
uint32_t score;

// ���������� "�������" �� �������
uint8_t doggie_x, doggie_y;

// ������������ ��������� �� �������
void DOG_MoveCursor(uint16_t digitX, uint16_t digitY)
{
  // ���������, ������������ �� �������
  char message[LCD_STR_LEN];

  // ��������� ����������� �������������� ������ ���� ����� ��������� �������
  if (digitX > (D_MAX / 2 + DELTA) || digitX < (D_MAX / 2 - DELTA)
  ||  digitY > (D_MAX / 2 + DELTA) || digitY < (D_MAX / 2 - DELTA))
  {
    // �������� ��������� � ������� �������
    LCD_PutSymbol(' ', symbol_x, symbol_y);

    // ��������� ��������� ��������� � ����������� �� ���������� ��������������
    // �������� ������� ������
    if (digitX > (D_MAX / 2 + DELTA))
    {
      // ��������� ���������� �� ��� �������
      symbol_x++;

      // �������� �� ����� �� ��������
      if (symbol_x > FIELD_X_MAX)
        symbol_x = FIELD_X_MAX;
    }

    // �������� ������� �����
    if (digitX < (D_MAX / 2 - DELTA))
    {
      // ��������� ���������� �� ��� �������
      symbol_x--;

      // �������� �� ����� �� ��������
      if (symbol_x < FIELD_X_MIN)
        symbol_x = FIELD_X_MIN;
    }

    // �������� ������� �����
    if (digitY > (D_MAX / 2 + DELTA))
    {
      // ��������� ���������� �� ��� �������
      symbol_y--;

      // �������� �� ����� �� ��������
      if (symbol_y < FIELD_Y_MIN)
        symbol_y = FIELD_Y_MIN;
    }

    // �������� ������� ����
    if (digitY < (D_MAX / 2 - DELTA))
    {
      // ��������� ���������� �� ��� �������
      symbol_y++;

      // �������� �� ����� �� ��������
      if (symbol_y > FIELD_Y_MAX)
        symbol_y = FIELD_Y_MAX;
    }

    // ����������� ��������� �� ����� �������
    LCD_PutSymbol('#', symbol_x, symbol_y);

    // ���� ��������� ����� "�������"...
    if (symbol_x == doggie_x && symbol_y == doggie_y)
    {
      // ������������ ��������������� ��������� "�������"
      while (symbol_x == doggie_x && symbol_y == doggie_y)
      {
        doggie_x = FIELD_X_MIN + rand() % (FIELD_X_MAX - FIELD_X_MIN + 1);
        doggie_y = FIELD_Y_MIN + rand() % (FIELD_Y_MAX - FIELD_Y_MIN + 1);
      }

      // ����������� "�������" �� ����� �������
      LCD_PutSymbol('@', doggie_x, doggie_y);

      // ���������� �������� �����
      score++;

      // ����������� ������ �������� ����� �� �������
      snprintf(message, LCD_STR_LEN, "    Score: %u", score);
      LCD_PutString(message, 7);

      // ������������ ������������ �������
      osTimerStart(TimerId_MoveDoggie, DOGGIE_MOVE_TIME);
    }
  }
}

// ����� ������������� ����
void Thread_DoggieInit(void *argument)
{
  // ������� �������
  LCD_ClearScreen();

  LCD_PutString("  Catch The Doggie!", 0);
  LCD_PutString("      Score: 0", 7);

  // ����������� �������������� ������ ����
  for (uint8_t i = FIELD_X_MIN - 1; i <= FIELD_X_MAX + 1; i++)
  {
    LCD_PutSymbol('-', i, FIELD_Y_MIN - 1);
    LCD_PutSymbol('-', i, FIELD_Y_MAX + 1);
  }

  // ����������� ������������ ������ ����
  for (uint8_t i = FIELD_Y_MIN; i <= FIELD_Y_MAX; i++)
  {
    LCD_PutSymbol('|', FIELD_X_MIN - 1, i);
    LCD_PutSymbol('|', FIELD_X_MAX + 1, i);
  }

  // ����������� ������� �� ��������� �������
  LCD_PutSymbol('#', symbol_x, symbol_y);

  // �������� ����� ��� ��������������� ������������������
  srand(MDR_ADC->ADC1_RESULT);

  // ������������ ��������������� ��������� ��� "�������"
  doggie_x = FIELD_X_MIN + rand() % (FIELD_X_MAX - FIELD_X_MIN + 1);
  doggie_y = FIELD_Y_MIN + rand() % (FIELD_Y_MAX - FIELD_Y_MIN + 1);

  // ����������� "�������"
  LCD_PutSymbol('@', doggie_x, doggie_y);

  // ����������� ��������� ������� ���� �� �������
  char message[LCD_STR_LEN];
  uint8_t t = DOGGIE_GAME_TIME / 1000;

  // �������������� ����� � ���������� ���
  snprintf(message, LCD_STR_LEN, "%02u:%02u", (t / 60), (t % 60));

  // ����������� ����������� ������� �� �������
  for (uint8_t i = 0; i < 5; i++)
    LCD_PutSymbol(message[i], 8 + i, 1);

  // ������ ����������� ��������
  osTimerStart(TimerId_MoveDoggie, DOGGIE_MOVE_TIME);
  osTimerStart(TimerId_TimeCounter, 1000);
  osTimerStart(TimerId_GameOver, DOGGIE_GAME_TIME);

  // �������� �������� ������
  osThreadTerminate(osThreadGetId);
}

// ��������� ������������ "�������", ���������� ��������
void TimerCallback_MoveDoggie(void *argument)
{
  // �������� "�������" � ������� ���������
  LCD_PutSymbol(' ', doggie_x, doggie_y);

  // ������������ ��������������� ��������� "�������"
  doggie_x = FIELD_X_MIN + rand() % (FIELD_X_MAX - FIELD_X_MIN + 1);
  doggie_y = FIELD_Y_MIN + rand() % (FIELD_Y_MAX - FIELD_Y_MIN + 1);

  // ��������� ������������ ���������, ���� ��� ������� � ������������ ���������
  while (symbol_x == doggie_x && symbol_y == doggie_y)
  {
    doggie_x = FIELD_X_MIN + rand() % (FIELD_X_MAX - FIELD_X_MIN + 1);
    doggie_y = FIELD_Y_MIN + rand() % (FIELD_Y_MAX - FIELD_Y_MIN + 1);
  }

  // ����������� "�������" �� ����� ����������
  LCD_PutSymbol('@', doggie_x, doggie_y);
}

// ��������� ������� ������� ����, ���������� ��������
void TimerCallback_TimeCounter(void *argument)
{
  // ���������, ������������ �� �������
  static char message[LCD_STR_LEN];

  // �������� ����� ���� � ��������� � �������
  static uint8_t t = DOGGIE_GAME_TIME / 1000;

  // ��������� �������
  t--;

  // �������������� ����� � ���������� ���
  snprintf(message, LCD_STR_LEN, "%02u:%02u", (t / 60), (t % 60));

  // ����������� ����������� ������� �� �������
  for (uint8_t i = 0; i < 5; i++)
    LCD_PutSymbol(message[i], 8 + i, 1);
}

// ��������� ���������� ����, ���������� ��������
void TimerCallback_GameOver(void *argument)
{
  char message[LCD_STR_LEN];

  // ��������� ������������ �������
  osKernelSuspend();

  // ��������� �������-��������� ���������������
  MDR_ADC->ADC1_CFG &= ~(1 << ADC1_CFG_REG_ADON_Pos);

  // ������� �������
  LCD_ClearScreen();

  // ���� � ������� ���������� ���������� ��������
  // ��������� ��������, �� ����������� ����� ��������
  if (MDR_BKP->REG_0D > 100)
    MDR_BKP->REG_0D = 0;

  // ���� ����� ���� �������� ���������,
  // �� �� ������������ � ������� ���������� ����������
  if (score > MDR_BKP->REG_0D)
    MDR_BKP->REG_0D = score;

  // ����������� �������� ����� � ������� �� �������
  snprintf(message, LCD_STR_LEN, "    Your Score: %u", score);
  LCD_PutString(message, 3);
  snprintf(message, LCD_STR_LEN, "     Record: %u", MDR_BKP->REG_0D);
  LCD_PutString(message, 5);
}
