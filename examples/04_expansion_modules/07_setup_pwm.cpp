/**
 * \file 07_setup_pwm.cpp
 * \brief Пример настройки и активации ШИМ-сигнала
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * 
 * В примере используется модуль DIO 8/8, настраивается дискретный выход DO1.
 * Настройка выхода начинается по команде "u setup_doutput" в терминале.
 * При наличии сигнала на дискретном входе DI1 на выходе DO1 формируются
 * импульсы с заданной длительностью и скважностью.
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите адрес модуля в module_dio
 *   - Установите настройки связи в modbus_master
 *   - Установите период .pwm_period и скважность .pwm_duty_cycle ШИМ в do_setup
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */
#include "pou_manager.h"

using namespace plc;


//! Модуль Alpha-X DIO
AlphaX_DIO alphax_module(1);

//! Список устройств Modbus для автоматического опроса
ModbusMasterPoll poll{
  alphax_module
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

//! Функционального блок настройки дискретных выходов
AlphaXModule_DiscreteOutputSetup do_setup ({
  // Modbus Master для отправки запроса
  .master               = &modbus_master,
  // Настраиваемое устройство
  .device               = &alphax_module,
  // Номер настраиваемого канала
  .channel              = AlphaXModule::DOUTPUT_CHANNEL1,
  // Записывать период ШИМ выхода
  .write_pwm_period     = true,
  // Записывать скважность ШИМ выхода
  .write_pwm_duty_cycle = true,
  // Записывать задержку включения (TON) выхода
  .write_ton            = true,
  // Записывать задержку выключения (TOF) выхода
  .write_tof            = true,
  // Период ШИМ, x * 100 мс
  .pwm_period           = 60,
  // Скважность ШИМ выхода, % * 100
  .pwm_duty_cycle       = 5000,
  // Задержка включения (TON) выхода, x * 1 с
  .ton                  = 0,
  // Задержка выключения (TOF) выхода, x * 1 с
  .tof                  = 0
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
  if(str == "setup_doutput")
  {
    do_setup.execute = true; // Команда "u save" для сохранения настроек
  }

  if(do_setup.done()) // Успешное выполнение функционального блока
  {
    do_setup.execute = false;
    print_debug("Discrete Output setup complete\r\n");
  }
  else if(do_setup.error()) // Ошибка в блоке функционального блока
  {
    do_setup.execute = false;
    print_debug("Discrete Output setup error id%d\n", do_setup.error_id());
  }
  else if (do_setup.idle() && do_setup.execute)
  {
    print_debug("Discrete Output begin setup\r\n");
  }
  do_setup(); // Выполнение функционального блока

  do1 = di1; // Включение ШИМ на выходе DO1 по сигналу с входа DI1
}
