/**
 * \file 06_reset_modules.cpp
 * \brief Пример сброса настроек модуля к заводским настройкам
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * В примере используется любой модуль Alpha-X (адрес 1).
 * Сброс начинается по команде "u reset" в терминале.
 * В модуле сбрасываются все настройки, кроме настроек связи Modbus.
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

//! Функциональный блок сброса модуля к заводским настройкам
AlphaXModule_FactoryReset factory_reset ({
  // Modbus Master для отправки запроса
  .master       = &modbus_master,
  // Настраиваемое устройство
  .device       = &alphax_module,
  // Сбрасывает настройки Modbus
  .reset_modbus = false
});

//! Строка для считывания пользовательской команды из терминала
etl::string<64> str;


void POU1()
{ 
  // Чтение пользовательской команды из терминала
  scan_debug(str);
  etl::to_lower_case(str);
  // Проверка пользовательской команды
  if(str == "reset")
  {
    factory_reset.execute = true; // Команда "u reset" для сброса модуля
  }

  if(factory_reset.done()) // Успешное выполнение функционального блока
  {
    factory_reset.execute = false;
    print_debug("Alpha-X Module reset complete\r\n");
  }
  else if(factory_reset.error()) // Ошибка в блоке функционального блока
  {
    factory_reset.execute = false;
    print_debug("Alpha-X Module reset error id%d\n", factory_reset.error_id());
  }
  else if (factory_reset.idle() && factory_reset.execute)
  {
    print_debug("Alpha-X Module begin reset\r\n");
  }
  factory_reset(); // Выполнение функционального блока
}
