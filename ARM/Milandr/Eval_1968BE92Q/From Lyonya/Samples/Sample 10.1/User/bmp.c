/*******************************************************************************
 * @File: bmp.c
 * @Author: Milandr, L.
 * @Project: Sample 10.1
 * @Microcontroller: 1986VE92U
 * @Device: Evaluation Board For MCU 1986VE92U
 * @Date: 19.05.2017
 * @Purpose: Обмен данными с цифровым датчиком давления и температуры BMP280
 *******************************************************************************/

// Подключение заголовочного файла
#include "bmp.h"

// Переменные для хранения калибровочных данных
uint16_t dig_T1;
int16_t  dig_T2;
int16_t  dig_T3;
uint16_t dig_P1;
int16_t  dig_P2;
int16_t  dig_P3;
int16_t  dig_P4;
int16_t  dig_P5;
int16_t  dig_P6;
int16_t  dig_P7;
int16_t  dig_P8;
int16_t  dig_P9;

// Переменная, используемая для компенсации значений давления в зависимости от температуры
BMP280_S32_t t_fine;

// Вычисление температуры в градусах Цельсия с точностью 0.01 °C
// Выходное значение "5123" эквивалентно 51.23 °C
// Функция взята из официальной документации на микросхему BMP280
BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
  BMP280_S32_t var1, var2, T;
  var1 = ((((adc_T >> 3) - ((BMP280_S32_t)dig_T1 << 1))) * ((BMP280_S32_t)dig_T2)) >> 11;
  var2 = (((((adc_T >> 4) - ((BMP280_S32_t)dig_T1)) * ((adc_T >> 4) - ((BMP280_S32_t)dig_T1))) >> 12) * ((BMP280_S32_t)dig_T3)) >> 14;
  t_fine = var1 + var2;
  T = (t_fine * 5 + 128) >> 8;
  return T;
}

// Вычисление давления в Паскалях
// Выходное значение "96386" эквивалентно 96386 Па
// Функция взята из официальной документации на микросхему BMP280
BMP280_U32_t bmp280_compensate_P_int32(BMP280_S32_t adc_P)
{
  BMP280_S32_t var1, var2;
  BMP280_U32_t p;
  var1 = (((BMP280_S32_t)t_fine) >> 1) - (BMP280_S32_t)64000;
  var2 = (((var1 >> 2) * (var1 >> 2)) >> 11 ) * ((BMP280_S32_t)dig_P6);
  var2 = var2 + ((var1 * ((BMP280_S32_t)dig_P5)) << 1);
  var2 = (var2 >> 2) + (((BMP280_S32_t)dig_P4) << 16);
  var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13 )) >> 3) + ((((BMP280_S32_t)dig_P2) * var1) >> 1)) >> 18;
  var1 =((((32768 + var1)) * ((BMP280_S32_t)dig_P1)) >> 15);
  if (var1 == 0)
    return 0; // avoid exception caused by division by zero
  p = (((BMP280_U32_t)(((BMP280_S32_t)1048576) - adc_P) - (var2 >> 12))) * 3125;
  if (p < 0x80000000)
    p = (p << 1) / ((BMP280_U32_t)var1);
  else
    p = (p / (BMP280_U32_t)var1) * 2;
  var1 = (((BMP280_S32_t)dig_P9) * ((BMP280_S32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
  var2 = (((BMP280_S32_t)(p >> 2)) * ((BMP280_S32_t)dig_P8)) >> 13;
  p = (BMP280_U32_t)((BMP280_S32_t)p + ((var1 + var2 + dig_P7) >> 4));
  return p;
}

// Получение калибровочных данных микросхемы BMP280
void BMP280_GetCalibrationData(void)
{
  uint8_t data[6];  // Массив принятых данных

  // Получение калибровочных данных
  SPI_ReceiveDataBurst(MDR_SSP1, BMP280_CALIB_T, 6, data);

  // Преобразование полученных данных в калибровочные константы
  dig_T1 = data[0] | (data[1] << 8);
  dig_T2 = data[2] | (data[3] << 8);
  dig_T3 = data[4] | (data[5] << 8);
}

// Инициализация микросхемы BMP280
void BMP280_Initialize(void)
{
  // Конфигурация порта для работы интерфейса SPI
  SPI_PortCfg();

  // Получение калибровочных данных
  BMP280_GetCalibrationData();

  // Конфигурация микросхемы BMP280
  // Настройка цифрового фильтра и интервала измерений
  SPI_TransmitData(MDR_SSP1, BMP280_CONFIG, BMP280_CONFIG_DATA);

  // Настройка точности измерений и режима работы
  SPI_TransmitData(MDR_SSP1, BMP280_CTRL_MEAS, BMP280_CTRL_MEAS_DATA);
}

// Поток работы с микросхемой BMP280
void Thread_BMP280(void *argument)
{
  uint8_t data[3];            // Массив принятых данных
  uint32_t digitT;            // Цифровое значение температуры
  float temperature;          // Температура
  char message[LCD_STR_LEN];  // Сообщение, отображаемое на дисплей

  // Инициализация микросхемы BMP280
  BMP280_Initialize();

  // Ожидание завершения первого преобразования
  osDelay(250);

  // Основной цикл
  while (1)
  {
    // Конфигурация линий ввода-вывода для работы с модулем SSP1
    SPI_PortCfg();

    // Чтение данных из регистров температуры
    SPI_ReceiveDataBurst(MDR_SSP1, BMP280_TEMP_MSB, 3, data);

    // Преобразование полученных данных в цифровое значение температуры
    digitT = (data[2] >> 4) | (data[1] << 4) | (data[0] << 12);

    // Преобразование цифрового значения температуры в градусы Цельсия
    temperature = (float)bmp280_compensate_T_int32(digitT) / 100;

    // Конфигурация линий ввода-вывода для работы с дисплеем
    LCD_PortCfg();

    // Отображение текущего значения температуры на дисплей
    snprintf(message, LCD_STR_LEN, "    T = %.2f°C", temperature);
    LCD_PutString(message, 4);

    // Задержка перед началом следующей итерации
    osDelay(250);
  }
}
