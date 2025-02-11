/**
 * \file 08_module_save_settings.cpp
 * \brief Пример сохранения настроек модулей Alpha-X в энергонезависимой памяти
 * модуля
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * 
 * В примере используется любой модуль Alpha-X (адрес 1).
 * Сохранение начинается по команде "u save" в терминале.
 * Пример сохраняет все настройки модуля, в т.ч. настройки связи.
 * 
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите адрес модуля в alphax_module
 *   - Установите настройки связи в modbus_master
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;


//! Модуль Alpha-X
AlphaXModule alphax_module(1);

//! Список устройств Modbus для автоматического опроса
ModbusMasterPoll poll(
  alphax_module
);

//! Функциональный блок Modbus Master на порту COM1
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

//! Функциональный блок сохранения настроек Alpha-X в энергонезависимую память
AlphaXModule_SaveParameters save_parameters ({
  // Modbus Master для отправки запроса
  .master        = &modbus_master,
  // Настраиваемое устройство
  .device        = &alphax_module,
  // Обновляет настройки Modbus после сохранения параметров
  .config_modbus = false
});

//! Строка для считывания пользовательской команды из терминала
etl::string<64> str;


void POU1()
{
  // Чтение пользовательской команды из терминала
  scan_debug(str);
  etl::to_lower_case(str);
  // Проверка пользовательской команды
  if(str == "save")
  {
    save_parameters.execute = true; // Команда "u save" для сохранения настроек
  }

  if (save_parameters.done()) // Успешное выполнение функционального блока
  {
    save_parameters.execute = false;
    print_debug("Alpha-X Module save complete\r\n");
  }
  else if (save_parameters.error()) // Ошибка выполнения функционального блока
  {
    save_parameters.execute = false;
    print_debug("Alpha-X Module save error id%d\n", save_parameters.error_id());
  }
  else if (save_parameters.idle() && save_parameters.execute)
  {
    print_debug("Alpha-X Module begin save\r\n");
  }
  save_parameters(); // Выполнение функционального блока
}
