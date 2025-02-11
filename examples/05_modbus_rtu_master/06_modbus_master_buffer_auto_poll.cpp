/**
 * \file 06_modbus_master_buffer_auto_poll.cpp
 * \brief Пример автоматического опроса с использованием буферов Modbus
 * \details
 * В примере реализуется автоматический опрос устройств с привязанным буфером
 * Modbus для стения и записи.
 * В примере используются 3 COM-порта ПЛК:
 * - COM1 настроен как Modbus Master
 * - COM2 настроен как Modbus Slave, который опрашивается Modbus Master
 * - COM3 настроен как Modbus Slave, отображающий результат выполнения
 * автоматического опроса
 * 
 * COM1 и COM2 должны быть соединены между собой. К COM3 подключается
 * пользователь.
 * 
 * Автоматический опрос читает и пишет через Modbus в буфер slave_buffer:
 * - При инициализации slave_buffer содержит значения регистров от 0 до 199
 * - Буфер в устройстве read_channel прочитает первые 100 регистров
 * slave_buffer и будет иметь значения регистров от 0 до 99
 * - Буфер в устройстве write_channel перезапишет вторые 100 регистров 
 * slave_buffer значениями от 1000 до 1099
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - Подключите COM1 и COM2 проводами как разные устройства Modbus
 * - Подключите COM3 к интерфейсу связи, используемым Вашим OPC-сервером
 * - Создайте конфигурацию в своем OPC сервере:
 *   - Чтение 200 регистров функцией 3 с адреса 0 - для отображения буфера
 *     slave_buffer
 *   - Чтение 100 регистров функцией 4 с адреса 0 - для отображения прочитанных
 *     данных read_channel
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Устройство с каналом автоматического чтения по Modbus
ModbusBufferDevice<100> read_channel ({
  // Адрес устройства Modbus
  .device_address {1},
  // Период опроса, мс (0 - опрос выключен)
  .poll_period    {10},
  // Флаг постоянной записи в устройство (false - постоянное чтение)
  .auto_write     {false},
  // Modbus функция записи
  .function_write {16},
  // Modbus функция чтения
  .function_read  {3},
  // Первый адрес регистра Modbus в канале опроса
  .register_start_address {0},
  // Максимальное кол-во регистров в одном запросе Modbus (0 - стандарт)
  .request_registers      {0}
});

//! Устройство с каналом автоматической записи по Modbus
ModbusBufferDevice<100> write_channel ({
  // Адрес устройства Modbus
  .device_address {1},
  // Период опроса, мс (0 - опрос выключен)
  .poll_period    {10},
  // Флаг постоянной записи в устройство (false - постоянное чтение)
  .auto_write     {true},
  // Modbus функция записи
  .function_write {16},
  // Modbus функция чтения
  .function_read  {3},
  // Первый адрес регистра Modbus в канале опроса
  .register_start_address {100},
  // Максимальное кол-во регистров в одном запросе Modbus (0 - стандарт)
  .request_registers      {0}
});

//! Список устройств для опроса по Modbus Master
ModbusMasterPoll poll{
  read_channel,
  write_channel
};

//! Функциональный блок Modbus Master на COM1
ModbusMaster modbus_master ({
  // Номер COM-порта
  .com              = COM1,
  // Список устройств для автоматического опроса
  .devices          = poll,
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
  .response_timeout = 300,
  // Количество попыток связи
  .failed_attempts  = 3,
  // Время восстановления опроса (мс)
  .restore_timeout  = 3000
});

//! Буфер Modbus Slave
ModbusBuffer<200> slave_buffer;

//! Функциональный блок Modbus Slave на COM2
ModbusSlave modbus_slave ({
  // Номер COM-порта
  .com               = COM2,
  // Таблица Holding Registers
  .holding_registers = &slave_buffer,
  // Таблица Input Registers
  .input_registers   = nullptr,
  // Таблица Coils
  .coils             = nullptr,
  // Таблица Discrete Inputs
  .discrete_inputs   = nullptr,
  // Адрес Slave в сети Modbus
  .address           = 1,
  // Скорость обмена, бит/с
  .baudrate          = 115200,
  // Четность
  .parity            = COM_PARITY_NONE,
  // Кол-во стоп-битов
  .stop_bits         = 1,
  // Пауза перед отправкой ответа (мс)
  .poll_delay        = 0
});

//! Функциональный блок Modbus Slave на COM3
ModbusSlave user_slave ({
  // Номер COM-порта
  .com               = COM3,
  // Таблица Holding Registers
  .holding_registers = &slave_buffer,
  // Таблица Input Registers
  .input_registers   = &read_channel,
  // Таблица Coils
  .coils             = nullptr,
  // Таблица Discrete Inputs
  .discrete_inputs   = nullptr,
  // Адрес Slave в сети Modbus
  .address           = 1,
  // Скорость обмена, бит/с
  .baudrate          = 115200,
  // Четность
  .parity            = COM_PARITY_NONE,
  // Кол-во стоп-битов
  .stop_bits         = 1,
  // Пауза перед отправкой ответа (мс)
  .poll_delay        = 0
});


void POU1()
{
  static bool init = false;
  if (!init)
  {
    init = true;
    // Инициализация переменных данной POU
    // Код в этом блоке выполнится 1 раз
    for(size_t i = 0; i < slave_buffer.size(); ++i)
    {
      slave_buffer.writeInt16(i, i);
    }
    for(size_t i = 0; i < write_channel.size(); ++i)
    {
      write_channel.writeInt16(i, 1000 + i);
    }
  }
}
