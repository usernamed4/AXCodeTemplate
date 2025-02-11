/**
 * \file 03_on-off_regulator.cpp
 * \brief Пример двухпозиционного регулятора
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * В примере используются модуль Alpha-X AI 8 (адрес 2). В качестве управляющего
 * выхода используется выход DO1 ПЛК.
 * 
 * Регулируемая величина считывается в аналоговом входе AI7,
 * уставка задается через считанное значение аналогового входа AI8.
 * Гистерезис регулятора задается через параметр ON_OFF_HYSTERESIS.
 * Состояние регулятора выводится в терминал.
 * 
 * Если выход DO1 включен:
 * - Выключение произойдет если AI7 больше или равен AI8.
 * Если выход DO1 включен:
 * - Выключение произойдет если AI7 меньше или равен (AI8 - ON_OFF_HYSTERESIS).
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите адрес модуля в module_ai8
 *   - Установите величину гистерезиса в ON_OFF_HYSTERESIS
 *   - Установите настройки связи в modbus_master
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 100, 1000);  // Период вызова 100 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Гистерезис двухпозиционного регулятора
#define ON_OFF_HYSTERESIS (5.0f)

//! Модуль Alpha-X AI 8
AlphaX_AI module_ai8(2);

//! Адреса устройств для автоматического опроса
ModbusMasterPoll poll{
  module_ai8
};

//! Инициализация параметров в сети Modbus
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

//! Дискретный выход ПЛК
DiscreteOutput do1(cpu_io, CPU_DOUTPUT1);

//! Аналоговый вход AI7
AnalogInput ai7(module_ai8, module_ai8.AINPUT_CHANNEL7);

//! Аналоговый вход AI8
AnalogInput ai8(module_ai8, module_ai8.AINPUT_CHANNEL8);


void POU1()
{
  static bool init = false;
  if (!init)
  {
    init = true;
    // По умолчанию регулятор включен
    do1 = true;
  }

  if(do1)
  {
    if(ai7 >= ai8)
    {
      do1 = false;
    }
  }
  else
  {
    if(ai7 <= (ai8 - ON_OFF_HYSTERESIS))
    {
      do1 = true;
    }
  }

  // Вывод состояния регулятора в терминал
  etl::string<32> str;
  etl::string_stream stream(str);
  stream << etl::setprecision(2)
         << "pv=" << etl::setw(6) << ai7.value() << etl::setw(0)
         << ", sv=" << etl::setw(6) << ai8.value() << etl::setw(0)
         << ", DO1=" << etl::boolalpha << do1.value() << "\n";
  print_debug(stream.str());
}
