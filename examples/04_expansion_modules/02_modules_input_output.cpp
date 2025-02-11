/**
 * \file 02_modules_input_output.cpp
 * \brief Пример включения дискретных входов в зависимости от уровня сигнала
 * на аналоговом входе
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * В примере используются модули:
 * - Alpha-X DO 16P (адрес 1)
 * - Alpha-X AI 8 (адрес 2)
 *
 * Дискретные выходы включаются как шкала в зависимости от значения
 * на аналоговом входе AI8:
 * - 0%   аналогового сигнала выключает все выходы
 * - 50%  аналогового сигнала включает первые 8 выходов
 * - 100% аналогового сигнала включает все выходы
 * Код, определяющий состояние выходов, запускается только при изменении
 * значения аналогового входа. В терминал будет выводиться считанное значение
 * аналогового входа, процент от макс. сигнала и кол-во включаемых выходов
 * 
 * На заводских настройках Alpha-X AI 8 датчик настроен на "0-10 В",
 * а пользовательский диапазон сигнала лежит в диапазоне от 0.0 до 100.00.
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Измените максимальный и минимальный уровни сигнала аналогового входа
 *   в MAX_ANALOG_INPUT и MIN_ANALOG_INPUT
 *   - Установите адреса модулей в module_do16p и module_ai8
 *   - Установите настройки связи в modbus_master
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 100, 1000);  // Период вызова 100 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Максимальный уровень сигнала аналогового входа (100%)
#define MAX_ANALOG_INPUT (100.0f)
//! Минимальный уровень сигнала аналгового входа (0%)
#define MIN_ANALOG_INPUT (0.0f)

//! Модуль Alpha-X DO 16P
AlphaX_DO module_do16p(1);
//! Модуль Alpha-X AI 8
AlphaX_AI module_ai8(2);

//! Список устройств Modbus для автоматического опроса
ModbusMasterPoll poll{
  module_do16p,
  module_ai8
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

//! Массив дискретных выходов
DiscreteOutputArray<module_do16p.DOUTPUT_CHANNEL_SIZE> dor(module_do16p);
//! Аналоговый вход AI8
AnalogInput ai(module_ai8, module_ai8.AINPUT_CHANNEL8);

//! Предыдущее значение аналогового входа (из прошлого цикла POU)
float old;
//! Кол-во работающих дискретных выходов
size_t graf;


void POU1()
{
  static bool init = false;
  // Код в этом блоке выполнится 1 раз
  if (!init)
  {
    init = true;
    // Значение для сравнения с текущем на аналоговом входе
    old = ai;
  }

  // Если значение аналогового входа изменилось
  if(ai != old)
  {
    // Обновляем предыдущее значение аналогового входа
    old = ai;
    // Вычисляем уровень аналогового сигнала в процентах
    float percent = (ai - MIN_ANALOG_INPUT) / MAX_ANALOG_INPUT * 100;
    // Вычисляем кол-во включаемых дискретных выходов
    graf = (int)percent * dor.size() / 100;
    if(graf > dor.size())
    {
      graf = dor.size();
    }

    // Установка выходов в зависимости от значения graf
    for(int i = 0; i < graf; i++) 
    {
      dor[i] = true;
    }
    for(size_t i = graf; i < dor.size(); i++)
    {
      dor[i] = false;
    }

    etl::string<128> str;
    etl::string_stream stream{str};

    stream << "ai=" << etl::setprecision(2) << ai.value()
           << ", percent=" << percent
           << ", outputs enabled=" << graf << "/" << dor.size() << "\n";
    print_debug(str);
  }
}
