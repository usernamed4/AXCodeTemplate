/**
 * \file 04_modbus_ecd2.cpp
 * \brief Примеры работы с ПИД-регулятором ELHART ECD2
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * 
 * Чтение значений регистров ПИД-регулятора ELHART ECD2 (адрес 1),
 * подключенного к интерфейсу COM2.
 * Считывает из прибора 194 регистра и сохраняет нужные значения
 * в отдельные переменные. Тип данных регистров - int16_t.
 * 
 * Для получения значения регистра, которого нет в примере, необходимо 
 * использовать метод:
 * \code{cpp}
 * int16_t my_variable = ecd_buffer.readInt16(ADDR);
 * \endcode
 * где ADDR - адрес регистра в приборе.
 * 
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите требуемые настройки связи в modbus_master
 *   - Установите адрес ECD2 .device_address в ecd_read
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;


//! Значение подаваемое на вход логического устройства (Каналы 1, 2)
int16_t input_value_lu[2]; 
//! Уставка логического устройства (Каналы 1, 2)
int16_t setvalue_lu1[2];
//! Вторая уставка (Каналы 1, 2)
int16_t setvalue_lu2[2];
//! Показания измерительного входа  (Каналы 1, 2)
int16_t input_value[2];
//! Тип подключаемого датчика (Каналы 1, 2)
int16_t sensor_type[2];
//! Логика работы логического устройства (Каналы 1, 2)
int16_t operation_logic_lu[2];
//! Режим работы логического устройства (Каналы 1, 2))
int16_t operating_mode_lu[2];
//! Автонастройка ПИД-регулятора (Каналы 1, 2)
int16_t auto_tuning[2];
//! Полоса пропорциональности (Каналы 1, 2)
int16_t xp[2];
//! Время интегрирования (Каналы 1, 2)
int16_t ti[2];
//! Время дифференцирования (Каналы 1, 2)
int16_t td[2];
//! Период ШИМ (Каналы 1, 2)
int16_t period_pwm[2];
//! Дополнительные функции входа PV2 (Каналы 1, 2)
int16_t additional_functions_pv2[2];
//! Ошибки (Каналы 1, 2)
int16_t er[2];
//! Состояние автоматической настройки (Каналы 1, 2)
int16_t auto_tuning_status[2]; 
//! Показания датчика температуры холодного спая
int16_t cold_junction; 
//! Скорость передачи данных по Modbus RTU
int16_t baud;

//! Карта регистров ECD2 
enum ECD2_MAP {
  //! Значение, подаваемое на вход логического устройства (Канал 1)
  VALUE_INPUT_LOGICAL_DEVICE_CH1 = 0,
  //! Значение, подаваемое на вход логического устройства (Канал 2)
  VALUE_INPUT_LOGICAL_DEVICE_CH2 = 1,
  //! Уставка логического устройства (Канал 1)
  LU_SV_1_CH1 = 3,
  //! Уставка логического устройства (Канал 2)
  LU_SV_1_CH2 = 4,
  //! Показание измерительного входа (Канал 1)
  MEASURING_INPUT_CH1 = 12,
  //! Показание измерительного входа (Канал 2)
  MEASURING_INPUT_CH2 = 13,
  //! Тип подключаемого датчика (Канал 1)
  SENSOR_TYPE_CH1 = 15,
  //! Тип подключаемого датчика (Канал 2)
  SENSOR_TYPE_CH2 = 16,
  //! Логика работы логического устройства (Канал 1)
  OPERATION_LOGIC_LU_CH1 = 48,
  //! Логика работы логического устройства (Канал 2)
  OPERATION_LOGIC_LU_CH2 = 49,
  //! Режим работы логического устройства (Канал 1)
  OPERATING_MODE_LU_CH1 = 51,
  //! Режим работы логического устройства (Канал 2)
  OPERATING_MODE_LU_CH2 = 52,
  //! Автонастройка ПИД-регулятора (Канал 1)
  PID_CONTROLLER_AUTO_TUNING_CH1 = 60,
  //! Автонастройка ПИД-регулятора (Канал 2)
  PID_CONTROLLER_AUTO_TUNING_CH2 = 61,
  //! Полоса пропорциональности (Канал 1)
  PROPORTIONAL_BAND_CH1 = 63,
  //! Полоса пропорциональности (Канал 2)
  PROPORTIONAL_BAND_CH2 = 64,
  //! Время интегрирования (Канал 1)
  TIME_INTEGRATION_CH1 = 66,
  //! Время интегрирования (Канал 2)
  TIME_INTEGRATION_CH2 = 67,
  //! Время дифференцирования (Канал 1)
  TIME_DEFFERENTIATION_CH1 = 69,
  //! Время дифференцирования (Канал 2)
  TIME_DEFFERENTIATION_CH2 = 70,
  //! Период ШИМ (Канал 1)
  PERIOD_PWM_CH1 = 75,
  //! Период ШИМ (Канал 2)
  PERIOD_PWM_CH2 = 76,
  //! Вторая уставка (Канал 1)
  LU_SV_2_CH1 = 102,
  //! Вторая уставка (Канал 2)
  LU_SV_2_CH2 = 103,
  //! Ошибки (Канал 1)
  ERROR_CH1 = 117,
  //! Ошибка (Канал 2)
  ERROR_CH2 = 118,
  //! Состояние автоматической настройки (Канал 1)
  AUTO_SETUP_STATUS_CH1 = 120,
  //! Состояние автоматической настройки (Канал 2)
  AUTO_SETUP_STATUS_CH2 = 121,
  //! Показания датчика температуры холодного спая
  COLD_JUNCTION_TEMPERATURE = 129,
  //! Скорость передачи данных по Modbus RTU
  BAUDRATE = 130,
  //! Дополнительные функции ИВ2 (Канал 1)
  ADDITIONAL_FUNCTIONS_PV2_CH1 = 179,
  //! Дополнительные функции ИВ2 (Канал 2)
  ADDITIONAL_FUNCTIONS_PV2_CH2 = 180,
};

//! Функциональный блок Modbus Master на порту COM2
ModbusMaster modbus_master ({
  // Номер COM-порта
  .com              = COM2,
  // Список устройств для автоматического опроса
  .devices          = {},
  // Скорость обмена, бит/с
  .baudrate         = 115200,
  // Четность
  .parity           = COM_PARITY_NONE,
  // Кол-во стоп-битов
  .stop_bits        = 1,
  // Пауза перед отправкой команды опроса (мс)
  .poll_delay       = 0,
  // Пауза после отправки широковещательного запроса (мс)
  .turnaround_delay = 100,
  // Время ожидания ответа (мс)
  .response_timeout = 500,
  // Количество попыток связи
  .failed_attempts  = 3,
  // Время восстановления опроса (мс)
  .restore_timeout  = 2000
});

//! Буфер считанных регистров ECD2
ModbusBuffer<194> ecd_buffer;

//! Функциональный блок опроса регистров ECD2
ModbusMasterRequest ecd_read ({
  // Modbus Master для отправки запроса
  .master               = &modbus_master,
  // Адрес опрашиваемого устройства
  .device_address       = 1,
  // Номер функции Modbus
  .function             = 3,
  // Начальный адрес регистров Modbus в запросе
  .starting_address     = 0,
  // Указатель на таблицу регистров
  .buffer               = &ecd_buffer,
  // Общее кол-во регистров Modbus
  .registers_count      = 0,
  // Максимальное кол-во регистров Modbus в одном запросе к устройству
  .device_max_registers = 0,
  // Время ожидания ответа в запросе, мс (0 - настройки из Modbus Master)
  .response_timeout     = 0
});


void POU1()
{
  ecd_read.execute = true; // Активация запроса 
  if (ecd_read.done()) // Успешное выполнение запроса
  {
    ecd_read.execute = false;

    // Сохранение значений регистров из буфера
    for(uint8_t i = 0; i < 2; ++i)
    {
      input_value_lu[i] = ecd_buffer.readInt16(
        ECD2_MAP::VALUE_INPUT_LOGICAL_DEVICE_CH1 + i
      );
      setvalue_lu1[i] = ecd_buffer.readInt16(ECD2_MAP::LU_SV_1_CH1 + i);
      setvalue_lu2[i] = ecd_buffer.readInt16(ECD2_MAP::LU_SV_2_CH1 + i);
      input_value[i]  = ecd_buffer.readInt16(ECD2_MAP::MEASURING_INPUT_CH1 + i);
      sensor_type[i]  = ecd_buffer.readInt16(ECD2_MAP::SENSOR_TYPE_CH1 + i);
      operation_logic_lu[i] = ecd_buffer.readInt16(
        ECD2_MAP::OPERATION_LOGIC_LU_CH1 + i
      );
      operating_mode_lu[i] = ecd_buffer.readInt16(
        ECD2_MAP::OPERATING_MODE_LU_CH1 + i
      );
      auto_tuning[i] = ecd_buffer.readInt16(
        ECD2_MAP::PID_CONTROLLER_AUTO_TUNING_CH1 + i
      );
      xp[i] = ecd_buffer.readInt16(ECD2_MAP::PROPORTIONAL_BAND_CH1 + i);
      ti[i] = ecd_buffer.readInt16(ECD2_MAP::TIME_INTEGRATION_CH1 + i);
      td[i] = ecd_buffer.readInt16(ECD2_MAP::TIME_DEFFERENTIATION_CH1 + i);
      period_pwm[i] = ecd_buffer.readInt16(ECD2_MAP::PERIOD_PWM_CH1 + i);
      er[i] = ecd_buffer.readInt16(ECD2_MAP::ERROR_CH1);
      auto_tuning_status[i] = ecd_buffer.readInt16(
        ECD2_MAP::AUTO_SETUP_STATUS_CH1 + 1
      );
      additional_functions_pv2[i] = ecd_buffer.readInt16(
        ECD2_MAP::ADDITIONAL_FUNCTIONS_PV2_CH1 + 1
      );
    }
    cold_junction = ecd_buffer.readInt16(ECD2_MAP::COLD_JUNCTION_TEMPERATURE);
    baud = ecd_buffer.readInt16(ECD2_MAP::BAUDRATE);
    print_debug("ECD2 read success\r\n");
  }
  else if (ecd_read.error()) // Ошибка выполнения запроса
  {
    ecd_read.execute = false;
    print_debug("ECD2 read error id%i\r\n", ecd_read.error_id());
  }
  ecd_read(); // Выполнение запроса 
}
