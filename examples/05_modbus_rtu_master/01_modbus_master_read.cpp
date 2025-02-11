/**
 * \file 01_modbus_master_read.cpp
 * \brief Пример считывания одного регистра Modbus
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * В примере реализуется считывание одного Modbus регистра в любом устройстве
 * Modbus (адрес 1), подключенного к интерфейсу COM2.
 * Результат выполнения запроса и прочитанный регистр выводятся в терминал.
 * 
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите требуемые настройки связи в modbus_master
 *   - Установите параметры запроса в modbus_read:
 *     - Адрес опрашиваемого устройства .device_address
 *     - Адрес регистра Modbus .starting_address
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;


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

//! Буфер Modbus
ModbusBuffer<1> modbus_read_buffer; 

//! Функциональный блок запроса Modbus
ModbusMasterRequest modbus_read ({
  // Modbus Master для отправки запроса
  .master               = &modbus_master,
  // Адрес опрашиваемого устройства
  .device_address       = 1,
  // Номер функции Modbus
  .function             = 3,
  // Начальный адрес регистров Modbus в запросе
  .starting_address     = 0,
  // Указатель на таблицу регистров
  .buffer               = &modbus_read_buffer,
  // Общее кол-во регистров Modbus
  .registers_count      = 0,
  // Максимальное кол-во регистров Modbus в одном запросе к устройству
  .device_max_registers = 0,
  // Время ожидания ответа в запросе, мс (0 - настройки из Modbus Master)
  .response_timeout     = 0
});


void POU1()
{
  modbus_read.execute = true;    // Активация запроса
  if (modbus_read.done())        // Успешное выполнение запроса
  {
    modbus_read.execute = false; // Сброс запроса
    print_debug(                 // Вывод значения регистра в терминал
      "Modbus read success, my_word: %d\n", modbus_read_buffer.readInt16(0)
    ); 
  }
  else if (modbus_read.error())  // Ошибка выполнения запроса
  {
    modbus_read.execute = false; // Сброс запроса 
    print_debug("Modbus read error id%i\r\n", modbus_read.error_id());
  }
  modbus_read();                 // Выполнение запроса
}
