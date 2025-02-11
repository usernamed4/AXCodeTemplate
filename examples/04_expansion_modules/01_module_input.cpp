/**
 * \file 01_module_input.cpp
 * \brief Пример вывода показаний аналогового входа в терминал
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * В примере используются:
 * - Модуль Alpha-X AI 8 с Modbus-адресом 2,
 * - Аналоговый вход, считываемый с входа №8 модуля Alpha-X AI 8
 * - Вывод в терминал значения аналогового входа
 * 
 * На заводских настройках аналогового входа в модуле в терминал будет 
 * отправляться уровень сигнала 0-10 В в процентах.
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите адрес модуля в module_ai8
 *   - Установите настройки связи в modbus_master
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 1000, 1000);  // Период вызова 1000 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Модуль Alpha-X AI 8
AlphaX_AI module_ai8(2);

//! Список устройств Modbus для автоматического опроса
ModbusMasterPoll poll{
  module_ai8
};

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

//! Аналоговый вход AI8 модуля Alpha-X AI 8
AnalogInput ai(module_ai8, module_ai8.AINPUT_CHANNEL8);
 

void POU1()
{
  etl::string<32> str;
  etl::string_stream stream{str};

  // Выводит в терминал значение аналогового входа с 2 знаками после запятой
  stream << "[" << etl::setprecision(2) << ai.value() << "]\r\n";
  print_debug(stream.str());
}
