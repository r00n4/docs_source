/*******************************************************************************
 * @File: dog.c
 * @Author: Milandr, L.
 * @Project: Sample 6.4
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 18.07.2018
 * @Purpose: Реализация алгоритма игры
 *******************************************************************************/

// Подключение заголовочного файла
#include "dog.h"

// Координаты указателя на дисплее
uint8_t symbol_x = FIELD_X_MIN;
uint8_t symbol_y = FIELD_Y_MIN;

// Счетчик игровых очков
uint32_t score;

// Координаты "собачки" на дисплее
uint8_t doggie_x, doggie_y;

// Передвижение указателя на дисплее
void DOG_MoveCursor(uint16_t digitX, uint16_t digitY)
{
  // Сообщение, отображаемое на дисплей
  char message[LCD_STR_LEN];

  // Обработка результатов преобразования только если рычаг джойстика сдвинут
  if (digitX > (D_MAX / 2 + DELTA) || digitX < (D_MAX / 2 - DELTA)
  ||  digitY > (D_MAX / 2 + DELTA) || digitY < (D_MAX / 2 - DELTA))
  {
    // Удаление указателя с текущей позиции
    LCD_PutSymbol(' ', symbol_x, symbol_y);

    // Изменение координат указателя в зависимости от результата преобразования
    // Джойстик сдвинут вправо
    if (digitX > (D_MAX / 2 + DELTA))
    {
      // Инкремент координаты по оси абсцисс
      symbol_x++;

      // Проверка на выход за диапазон
      if (symbol_x > FIELD_X_MAX)
        symbol_x = FIELD_X_MAX;
    }

    // Джойстик сдвинут влево
    if (digitX < (D_MAX / 2 - DELTA))
    {
      // Декремент координаты по оси абсцисс
      symbol_x--;

      // Проверка на выход за диапазон
      if (symbol_x < FIELD_X_MIN)
        symbol_x = FIELD_X_MIN;
    }

    // Джойстик сдвинут вверх
    if (digitY > (D_MAX / 2 + DELTA))
    {
      // Декремент координаты по оси ординат
      symbol_y--;

      // Проверка на выход за диапазон
      if (symbol_y < FIELD_Y_MIN)
        symbol_y = FIELD_Y_MIN;
    }

    // Джойстик сдвинут вниз
    if (digitY < (D_MAX / 2 - DELTA))
    {
      // Инкремент координаты по оси ординат
      symbol_y++;

      // Проверка на выход за диапазон
      if (symbol_y > FIELD_Y_MAX)
        symbol_y = FIELD_Y_MAX;
    }

    // Отображение указателя на новую позицию
    LCD_PutSymbol('#', symbol_x, symbol_y);

    // Если указатель нашел "собачку"...
    if (symbol_x == doggie_x && symbol_y == doggie_y)
    {
      // Формирование псевдослучайных координат "собачки"
      while (symbol_x == doggie_x && symbol_y == doggie_y)
      {
        doggie_x = FIELD_X_MIN + rand() % (FIELD_X_MAX - FIELD_X_MIN + 1);
        doggie_y = FIELD_Y_MIN + rand() % (FIELD_Y_MAX - FIELD_Y_MIN + 1);
      }

      // Отображение "собачки" на новую позицию
      LCD_PutSymbol('@', doggie_x, doggie_y);

      // Увеличение счетчика очков
      score++;

      // Отображение нового значения счета на дисплей
      snprintf(message, LCD_STR_LEN, "    Score: %u", score);
      LCD_PutString(message, 7);

      // Перезагрузка виртуального таймера
      osTimerStart(TimerId_MoveDoggie, DOGGIE_MOVE_TIME);
    }
  }
}

// Поток инициализации игры
void Thread_DoggieInit(void *argument)
{
  // Очистка дисплея
  LCD_ClearScreen();

  LCD_PutString("  Catch The Doggie!", 0);
  LCD_PutString("      Score: 0", 7);

  // Отображение горизонтальных границ поля
  for (uint8_t i = FIELD_X_MIN - 1; i <= FIELD_X_MAX + 1; i++)
  {
    LCD_PutSymbol('-', i, FIELD_Y_MIN - 1);
    LCD_PutSymbol('-', i, FIELD_Y_MAX + 1);
  }

  // Отображение вертикальных границ поля
  for (uint8_t i = FIELD_Y_MIN; i <= FIELD_Y_MAX; i++)
  {
    LCD_PutSymbol('|', FIELD_X_MIN - 1, i);
    LCD_PutSymbol('|', FIELD_X_MAX + 1, i);
  }

  // Отображение символа на начальную позицию
  LCD_PutSymbol('#', symbol_x, symbol_y);

  // Создание зерна для псевдослучайной последовательности
  srand(MDR_ADC->ADC1_RESULT);

  // Формирование псевдослучайных координат для "собачки"
  doggie_x = FIELD_X_MIN + rand() % (FIELD_X_MAX - FIELD_X_MIN + 1);
  doggie_y = FIELD_Y_MIN + rand() % (FIELD_Y_MAX - FIELD_Y_MIN + 1);

  // Отображение "собачки"
  LCD_PutSymbol('@', doggie_x, doggie_y);

  // Отображение исходного времени игры на дисплей
  char message[LCD_STR_LEN];
  uint8_t t = DOGGIE_GAME_TIME / 1000;

  // Преобразование числа в символьный тип
  snprintf(message, LCD_STR_LEN, "%02u:%02u", (t / 60), (t % 60));

  // Отображение оставшегося времени на дисплей
  for (uint8_t i = 0; i < 5; i++)
    LCD_PutSymbol(message[i], 8 + i, 1);

  // Запуск виртуальных таймеров
  osTimerStart(TimerId_MoveDoggie, DOGGIE_MOVE_TIME);
  osTimerStart(TimerId_TimeCounter, 1000);
  osTimerStart(TimerId_GameOver, DOGGIE_GAME_TIME);

  // Удаление текущего потока
  osThreadTerminate(osThreadGetId);
}

// Процедура передвижения "собачки", вызываемая таймером
void TimerCallback_MoveDoggie(void *argument)
{
  // Удаление "собачки" с текущих координат
  LCD_PutSymbol(' ', doggie_x, doggie_y);

  // Формирование псевдослучайных координат "собачки"
  doggie_x = FIELD_X_MIN + rand() % (FIELD_X_MAX - FIELD_X_MIN + 1);
  doggie_y = FIELD_Y_MIN + rand() % (FIELD_Y_MAX - FIELD_Y_MIN + 1);

  // Повторное формирование координат, если они совпали с координатами указателя
  while (symbol_x == doggie_x && symbol_y == doggie_y)
  {
    doggie_x = FIELD_X_MIN + rand() % (FIELD_X_MAX - FIELD_X_MIN + 1);
    doggie_y = FIELD_Y_MIN + rand() % (FIELD_Y_MAX - FIELD_Y_MIN + 1);
  }

  // Отображение "собачки" на новые координаты
  LCD_PutSymbol('@', doggie_x, doggie_y);
}

// Процедура отсчета времени игры, вызываемая таймером
void TimerCallback_TimeCounter(void *argument)
{
  // Сообщение, отображаемое на дисплей
  static char message[LCD_STR_LEN];

  // Заданное время игры с переводом в секунды
  static uint8_t t = DOGGIE_GAME_TIME / 1000;

  // Декремент времени
  t--;

  // Преобразование числа в символьный тип
  snprintf(message, LCD_STR_LEN, "%02u:%02u", (t / 60), (t % 60));

  // Отображение оставшегося времени на дисплей
  for (uint8_t i = 0; i < 5; i++)
    LCD_PutSymbol(message[i], 8 + i, 1);
}

// Процедура завершения игры, вызываемая таймером
void TimerCallback_GameOver(void *argument)
{
  char message[LCD_STR_LEN];

  // Остановка операционной системы
  osKernelSuspend();

  // Остановка аналого-цифрового преобразователя
  MDR_ADC->ADC1_CFG &= ~(1 << ADC1_CFG_REG_ADON_Pos);

  // Очистка дисплея
  LCD_ClearScreen();

  // Если в регистр аварийного сохранения содержит
  // стороннее значение, то выполняется сброс регистра
  if (MDR_BKP->REG_0D > 100)
    MDR_BKP->REG_0D = 0;

  // Если новый счет является рекордным,
  // то он записывается в регистр аварийного сохранения
  if (score > MDR_BKP->REG_0D)
    MDR_BKP->REG_0D = score;

  // Отображение значений счета и рекорда на дисплей
  snprintf(message, LCD_STR_LEN, "    Your Score: %u", score);
  LCD_PutString(message, 3);
  snprintf(message, LCD_STR_LEN, "     Record: %u", MDR_BKP->REG_0D);
  LCD_PutString(message, 5);
}
