/**
 * \file 04_setup_dinput.cpp
 * \brief Пример настройки дискретных входов
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * 
 * В примере настраивается дискретный вход DI1 модуля Alpha-X DIO 8/8P
 * (Адрес 1). Настройка начинается по команде "u setup_dinput" в терминале.
 * 
 * Вход настраивается на задержку включения (2,5с) и выключения (0,25с).
 * Дискретный выход DO1 повторяет состояние выхода DI1. 
 * 
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите адрес модуля в module_dio
 *   - Установите настройки связи в modbus_master
 *   - Установите задержку включения .ton и задержку выключения .tof в di_setup
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Модуль Alpha-X DIO 8/8P
AlphaX_DIO alphax_module(1);

//! Список устройств Modbus для автоматического опроса
ModbusMasterPoll poll{
  alphax_module
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

//! Функциональный блок настройки дискретных входов
AlphaXModule_DiscreteInputSetup di_setup ({
  // Modbus Master для отправки запроса
  .master    = &modbus_master,
  // Настраиваемое устройство
  .device    = &alphax_module,
  // Номер настраиваемого канала
  .channel   = AlphaXModule::DINPUT_CHANNEL1,
  // Записывать задержку включения (TON) DI
  .write_ton = true,
  // Записывать задержку выключения (TOF) DI
  .write_tof = true,
  // Задержка включения (TON) DI, x * 500 мкс
  .ton       = 5000,
  // Задержка выключения (TOF) DI, x * 500 мкс
  .tof       = 500
});

//! Дискретный вход DI1 модуля расширения
DiscreteInput  di1(alphax_module, alphax_module.DINPUT_CHANNEL1);
//! Дискретный выход DO1 модуля расширения
DiscreteOutput do1(alphax_module, alphax_module.DOUTPUT_CHANNEL1);

//! Строка для считывания пользовательской команды из терминала
etl::string<64> str;


void POU1()
{
  // Чтение пользовательской команды из терминала
  scan_debug(str);
  etl::to_lower_case(str);
  // Проверка пользовательской команды
  if(str == "setup_dinput") // Команда настройки дискретного входа
  {
    // Команда "u setup_dinput" для настройки дискретного входа
    di_setup.execute = true;
  }

  if (di_setup.done()) // Успешное выполнение функционального блока
  {
    di_setup.execute = false;
    print_debug("Setup complete\r\n");
  }
  else if (di_setup.error()) // Ошибка в блоке функционального блока
  {
    di_setup.execute = false; // Сброс функционального блока 
    print_debug("Setup error id%d", di_setup.error_id());
  }
  else if (di_setup.idle() && di_setup.execute)
  {
    print_debug("Setup started\r\n");
  }
  di_setup(); // Выполнение функционального блока

  do1 = di1;
}
