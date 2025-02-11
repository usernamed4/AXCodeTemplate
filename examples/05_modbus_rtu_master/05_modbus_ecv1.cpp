/**
 * \file 05_modbus_ecv1.cpp
 * \brief Примеры работы с регулятором ELHART ECV1
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * 
 * Чтение значений регистров регулятора ELHART ECV1 (адрес 1),
 * подключенного к интерфейсу COM2.
 * Считывает из прибора 207 регистров и сохраняет нужные значения в отдельные
 * переменные. Тип данных регистров - int16_t.
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
 *   - Установите адрес ECV1 .device_address в ecv_read
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;


//! Значение, подаваемое на вход ЛУ1
int16_t input_value_lu1;
//! Значение, подаваемое на вход ЛУ2
int16_t input_value_lu2;
//! Уставка ЛУ1
int16_t setvalue_lu1;
//! Уставка ЛУ2
int16_t setvalue_lu2;
//! Значение измеренное на входе ИВ1
int16_t input_value_1;
//! Значение измеренное на входе ИВ2
int16_t input_value_2;
//! ПУСК/СТОП ЛУ1
int16_t on_off_lu1;
//! ПУСК/СТОП ЛУ2
int16_t on_off_lu2;
//! Положение задвижки
int16_t gate_position;
//! Выбор типа подключаемого датчика Входа ИВ1
int16_t sensor_type_1;
//! Выбор типа подключаемого датчика Входа ИВ2
int16_t sensor_type_2;
//! Выбор выходного сигнала ЛУ1
int16_t signal_selection_lu1;
//! Логика работы ЛУ1
int16_t operation_logic_lu1;
//! Режим работы ЛУ1
int16_t operating_mode_lu1;
//! Тип автонастройки
int16_t auto_tuning_type;
//! Полоса пропорциональности
int16_t xp;
//! Время интегрирования
int16_t ti;
//! Время дифференцирования
int16_t td;
//! Выбор выходного сигнала ЛУ1
int16_t signal_selection_lu2;
//! Логика работы ЛУ2
int16_t operation_logic_lu2;
//! Режим работы ЛУ2
int16_t operating_mode_lu2;
//! Период ШИМ 1
int16_t period_pwm_1;
//! Период ШИМ 2
int16_t period_pwm_2;
//! Показания датчика температуры холодного спая
int16_t cold_junction;
//! Ошибка 1
int16_t er1;
//! Ошибка 2
int16_t er2;


//! Карта регистров ECV1
enum ECV1_MAP {
  //! Значение, подаваемое на вход ЛУ2
  VALUE_INPUT_LU1 = 0,
  //! Значение, подаваемое на вход ЛУ2
  VALUE_INPUT_LU2 = 1,
  //! Уставка ЛУ1
  LU1_SV = 3,
  //! Уставка ЛУ2
  LU2_SV = 4,
  //! Значение, измеренное на входе ИВ1
  MEASURING_INPUT_1 = 18,
  //! Значение, измеренное на входе ИВ2
  MEASURING_INPUT_2 = 19,
  //! ПУСК/СТОП ЛУ1
  START_STOP_LU1 = 21,
  //! ПУСК/СТОП ЛУ2
  START_STOP_LU2 = 22,
  //! Положение задвижки
  GATE_POSITION = 27,
  //! Выбор типа подключаемого датчика Входа ИВ1
  SENSOR_TYPE_1 = 44,
  //! Выбор типа подключаемого датчика Входа ИВ2
  SENSOR_TYPE_2 = 45,
  //! Выбор входного сигнала ЛУ1
  SELECTION_SIGNAL_LU1 = 83,
  //! Выбор входного сигнала ЛУ2
  SELECTION_SIGNAL_LU2 = 84,
  //! Логика работы ЛУ1
  OPERATION_LOGIC_LU1 = 92,  
  //! Логика работы ЛУ2
  OPERATION_LOGIC_LU2 = 93,
  //! Режим работы ЛУ1
  OPERATING_MODE_LU1 = 95,
  //! Режим работы ЛУ2
  OPERATING_MODE_LU2 = 96,
  //! Тип автонастройки 
  AUTO_SETTING_TYPE = 98,
  //! Xp - полоса пропорциональности
  PROPORTIONAL_BAND = 100,
  //! Ti - время интегрирования
  TIME_INTEGRATION = 102,
  //! Td - время дифференцирования
  TIME_DEFFERENTIATION = 104,
  //! Период ШИМ 1
  PERIOD_PWM_1 = 134,
  //! Период ШИМ 2
  PERIOD_PWM_2 = 135,
  //! Показания датчика температуры холодного спая
  COLD_JUNCTION_TEMPERATURE = 184,
  //! Ошибка 1
  ERROR_1 = 197,
  //! Ошибка 2
  ERROR_2 = 198,
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

//! Буфер считанных регистров ECV1
ModbusBuffer<207> ecv_buffer;

//! Функциональный блок опроса регистров ECD2
ModbusMasterRequest ecv_read ({
  // Modbus Master для отправки запроса
  .master               = &modbus_master,
  // Адрес опрашиваемого устройства
  .device_address       = 1,
  // Номер функции Modbus
  .function             = 3,
  // Начальный адрес регистров Modbus в запросе
  .starting_address     = 0,
  // Указатель на таблицу регистров
  .buffer               = &ecv_buffer,
  // Общее кол-во регистров Modbus
  .registers_count      = 0,
  // Максимальное кол-во регистров Modbus в одном запросе к устройству
  .device_max_registers = 0,
  // Время ожидания ответа в запросе, мс (0 - настройки из Modbus Master)
  .response_timeout     = 0
});


void POU1()
{
  ecv_read.execute = true; // Активация запроса 
  if (ecv_read.done()) // Успешное выполнение запроса
  {
    ecv_read.execute = false;

    // Сохранение значений регистров из буфера
    input_value_lu1 = ecv_buffer.readInt16(ECV1_MAP::VALUE_INPUT_LU1);
    input_value_lu2 = ecv_buffer.readInt16(ECV1_MAP::VALUE_INPUT_LU2);
    setvalue_lu1    = ecv_buffer.readInt16(ECV1_MAP::LU1_SV);
    setvalue_lu2    = ecv_buffer.readInt16(ECV1_MAP::LU2_SV);
    input_value_1   = ecv_buffer.readInt16(ECV1_MAP::MEASURING_INPUT_1);
    input_value_2   = ecv_buffer.readInt16(ECV1_MAP::MEASURING_INPUT_2);
    on_off_lu1      = ecv_buffer.readInt16(ECV1_MAP::START_STOP_LU1);
    on_off_lu2      = ecv_buffer.readInt16(ECV1_MAP::START_STOP_LU2);
    gate_position   = ecv_buffer.readInt16(ECV1_MAP::GATE_POSITION);
    sensor_type_1   = ecv_buffer.readInt16(ECV1_MAP::SENSOR_TYPE_1);
    sensor_type_2   = ecv_buffer.readInt16(ECV1_MAP::SENSOR_TYPE_2);
    signal_selection_lu1 = ecv_buffer.readInt16(ECV1_MAP::SELECTION_SIGNAL_LU1);
    operation_logic_lu1  = ecv_buffer.readInt16(ECV1_MAP::OPERATION_LOGIC_LU1);
    operating_mode_lu1   = ecv_buffer.readInt16(ECV1_MAP::OPERATING_MODE_LU1);
    auto_tuning_type     = ecv_buffer.readInt16(ECV1_MAP::AUTO_SETTING_TYPE);
    xp = ecv_buffer.readInt16(ECV1_MAP::PROPORTIONAL_BAND);
    ti = ecv_buffer.readInt16(ECV1_MAP::TIME_INTEGRATION);
    td = ecv_buffer.readInt16(ECV1_MAP::TIME_DEFFERENTIATION);
    signal_selection_lu2 = ecv_buffer.readInt16(ECV1_MAP::SELECTION_SIGNAL_LU2);
    operation_logic_lu2  = ecv_buffer.readInt16(ECV1_MAP::OPERATION_LOGIC_LU2);
    operating_mode_lu2   = ecv_buffer.readInt16(ECV1_MAP::OPERATING_MODE_LU2);
    period_pwm_1  = ecv_buffer.readInt16(ECV1_MAP::PERIOD_PWM_1);
    period_pwm_2  = ecv_buffer.readInt16(ECV1_MAP::PERIOD_PWM_2);
    cold_junction = ecv_buffer.readInt16(ECV1_MAP::COLD_JUNCTION_TEMPERATURE);
    er1           = ecv_buffer.readInt16(ECV1_MAP::ERROR_1);
    er2           = ecv_buffer.readInt16(ECV1_MAP::ERROR_2);
    print_debug("ECV1 read success\r\n");
  }
  else if (ecv_read.error()) // Ошибка выполнения запроса
  {
    ecv_read.execute = false;
    print_debug("ECV1 read error id%i\r\n", ecv_read.error_id());
  }
  ecv_read(); // Выполнение запроса 
}
