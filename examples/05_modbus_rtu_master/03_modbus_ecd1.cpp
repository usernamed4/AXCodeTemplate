/**
 * \file 03_modbus_ecd1.cpp
 * \brief Пример считывания показаний измерительного входа ECD1 по Modbus
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * 
 * Считывает показания прибора ECD1 (адрес 1), подключенного к интерфейсу COM2.
 * Считанное значение выводится в терминал с одним знаком после запятой
 * (параметр 1-09 в приборе ECD должен быть равен 1) 
 * 
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите требуемые настройки связи в modbus_master
 *   - Установите адрес ECD1 .device_address в request_temperature
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Считанная температура из ECD1
volatile int16_t temp = 0; 

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
  .response_timeout = 300,
  // Количество попыток связи
  .failed_attempts  = 3,
  // Время восстановления опроса (мс)
  .restore_timeout  = 3000
});

//! Буфер для хранения температуры из ECD1
ModbusBuffer<1> request_buffer; 

//! Функциональный блок чтения показаний входа (регистр 0) из ECD1 по Modbus
ModbusMasterRequest request_temperature ({
  // Modbus Master для отправки запроса
  .master               = &modbus_master,
  // Адрес опрашиваемого устройства
  .device_address       = 1,
  // Номер функции Modbus
  .function             = 3,
  // Начальный адрес регистров Modbus в запросе
  .starting_address     = 0,
  // Указатель на таблицу регистров
  .buffer               = &request_buffer,
  // Общее кол-во регистров Modbus
  .registers_count      = 0,
  // Максимальное кол-во регистров Modbus в одном запросе к устройству
  .device_max_registers = 0,
  // Время ожидания ответа в запросе, мс (0 - настройки из Modbus Master)
  .response_timeout     = 0
});


void POU1()
{
  request_temperature.execute = true; // Активация запроса
  if (request_temperature.done())     // Успешное выполнение запроса
  {
    // Сброс запроса
    request_temperature.execute = false; 
    // Чтение значения температуры в целочисленном формате
    temp = request_buffer.readInt16(0);
    // Вывод температуры в терминал (с одним знаком после запятой)
    print_debug("Temperature: %i,%i\n", temp / 10, temp % 10);
  }
  else if (request_temperature.error()) // Ошибка выполнения запроса
  {
    request_temperature.execute = false;
    print_debug(
      "Temperature read error id%i\r\n", request_temperature.error_id()
    );
  }
  request_temperature(); // Выполнение запроса
}
