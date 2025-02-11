/**
 * \file 02_modbus_master_write.cpp
 * \brief Пример записи одного регистра Modbus
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * В примере реализуется запись одного Modbus регистра в любом устройстве
 * Modbus (адрес 1), подключенного к интерфейсу COM2.
 * Результат выполнения запроса выводится в терминал.
 * Записываемый регистр можно поменять через терминал
 * командой "u значение_регистра", например "u 123".
 * 
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите требуемые настройки связи в modbus_master
 *   - Установите параметры запроса в modbus_read:
 *     - Адрес опрашиваемого устройства .device_address
 *     - Адрес регистра Modbus .starting_address
 *   - Установите записываемое значение регистра в my_word
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Записываемое значение регистра
int16_t my_word = 100; 

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
ModbusBuffer<1> modbus_write_buffer; 

//! Функциональный блок запроса Modbus
ModbusMasterRequest modbus_write ({
  // Modbus Master для отправки запроса
  .master               = &modbus_master,
  // Адрес опрашиваемого устройства
  .device_address       = 1,
  // Номер функции Modbus
  .function             = 6,
  // Начальный адрес регистров Modbus в запросе
  .starting_address     = 0,
  // Указатель на таблицу регистров
  .buffer               = &modbus_write_buffer,
  // Общее кол-во регистров Modbus
  .registers_count      = 0, // 0 - по размеру таблицы регистров
  // Максимальное кол-во регистров Modbus в одном запросе к устройству
  .device_max_registers = 0, // 0 - по стандарту Modbus
  // Время ожидания ответа в запросе, мс (0 - настройки из Modbus Master)
  .response_timeout     = 0
});


void POU1()
{
  // Чтение из терминала нового значения регистра
  etl::string<16> str;
  scan_debug(str);
  if (str.empty() == false)
  {
    int result = sscanf(str.c_str(), "%hd", &my_word);
    if (result == 1)
    {
      // Функция должна правильно извлечь 1 переменную из строки
      print_debug("New register value: %d\r\n", my_word);
    }
    else
    {
      // Неизвестное значение аргумента в команде
      print_debug("Wrong user command\r\n");
    }
  }

  if (modbus_write.done())        // Успешное выполнение запроса
  {
    modbus_write.execute = false; // Сброс запроса
    print_debug(                  // Вывод значения регистра в терминал
      "Modbus write success, my_word: %d\r\n", modbus_write_buffer.readInt16(0)
    ); 
  }
  else if (modbus_write.error())  // Ошибка выполнения запроса
  {
    modbus_write.execute = false;
    print_debug("Modbus write error id%i\r\n", modbus_write.error_id());
  }
  else if (modbus_write.idle())   // Запрос не выполняется
  {
    modbus_write.execute = true;                // Активация запроса
    modbus_write_buffer.writeInt16(0, my_word); // Запись значения регистра
    print_debug("Starting modbus write\r\n");
  }
  modbus_write(); // Выполнение запроса
}
