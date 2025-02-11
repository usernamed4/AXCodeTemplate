/**
 * \file 05_setup_analog_input.cpp
 * \brief Пример настройки аналоговых входов
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * 
 * В примере используется модуль AI 6T (адрес 1), настраивается аналоговый
 * вход AI1. Настройка начинается по команде "u setup_ainput" в терминале.
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите адрес модуля в module_ai
 *   - Установите настройки связи в modbus_master
 *   - Установите записываемые параметры в ai_setup
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */
#include "pou_manager.h"

using namespace plc;

//! Модуль Alpha-X AI 6T
AlphaX_AI module_ai(1);

//! Список устройств Modbus для автоматического опроса
ModbusMasterPoll poll(
  module_ai
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

//! Функциональный блок настройки аналоговых входов
AlphaXModule_AnalogInputSetup ai_setup ({
  // Modbus Master для отправки запроса
  .master               = &modbus_master,
  // Настраиваемое устройство
  .device               = &module_ai,
  // Номер настраиваемого канала
  .channel              = AlphaXModule::AINPUT_CHANNEL1,
  // Записывать тип датчика
  .write_type           = true,
  // Записывать степень фильтрации сигнала
  .write_filter         = true,
  // Записывать нижнее значение для приведения к пользовательской шкале
  .write_normalize_low  = true,
  // Записывать верхнее значение для приведения к пользовательской шкале
  .write_normalize_high = true,
  // Записывать нижний уровень аналогового сигнала
  .write_level_low      = true,
  // Записывать верхний уровень аналогового сигнала
  .write_level_high     = true,
  // Записывать нижнее аварийное значение сигнала
  .write_alarm_low      = true,
  // Записывать верхнее аварийное значение сигнала
  .write_alarm_high     = true,
  // Записывать наклон характеристики измерительного входа
  .write_slope          = true,
  // Записывать сдвиг характеристики измерительного входа
  .write_shift          = true,
  // Записывать положение десятичной точки аналогового входа
  .write_decimal        = true,
  // Записывать отключение режекторного фильтра
  .write_disable_notch_filter = true,
  // Записывать компенсацию температуры холодного спая
  .write_junction       = true,
  // Тип подключаемого датчика
  .type                 = alphax_parameter::AINPUT_TYPE::SENSOR_50M,
  // Степень фильтрации сигнала
  .filter               = 2,
  // Нижнее значение для приведения к пользовательской шкале (ед. изм.)
  .normalize_low        = 0,
  // Верхнее значение для приведения к пользовательской шкале (ед. изм.)
  .normalize_high       = 10000,
  // Коррекция нижнего уровня аналогового сигнала
  .level_low            = 0,
  // Коррекция верхнего уровня аналогового сигнала
  .level_high           = 10000,
  // Нижнее аварийное значение сигнала в пользовательском диапазоне
  .alarm_low            = 0,
  // Верхнее аварийное значение сигнала в пользовательском диапазоне
  .alarm_high           = 10000,
  // Наклон характеристики измерительного входа
  .slope                = 1000,
  // Сдвиг характеристики измерительного входа, (ед. изм.)
  .shift                = 0,
  // Положение десятичной точки аналогового входа
  .decimal              = 2,
  // Отключение режекторного фильтра
  .disable_notch_filter = false,
  // Компенсация температуры холодного спая
  .junction             = alphax_parameter::AINPUT_JUNCTION::INTERNAL,
});

//! Строка для считывания пользовательской команды из терминала
etl::string<64> str;


void POU1()
{
  // Чтение пользовательской команды из терминала
  scan_debug(str);
  etl::to_lower_case(str);
  // Проверка пользовательской команды
  if(str == "setup_ainput") // Команда настройки дискретного входа
  {
    // Команда "u setup_ainput" для настройки дискретного входа
    ai_setup.execute = true;
  }

  if (ai_setup.done()) // Успешное выполнение функционального блока
  {
    ai_setup.execute = false;
    print_debug("Setup complete\r\n");
  }
  else if (ai_setup.error()) // Ошибка в блоке функционального блока
  {
    ai_setup.execute = false;
    print_debug("Setup error id%i\r\n", ai_setup.error_id());
  }
  else if (ai_setup.idle() && ai_setup.execute)
  {
    print_debug("Setup started\r\n");
  }
  ai_setup(); // Выполнение функционального блока
}
