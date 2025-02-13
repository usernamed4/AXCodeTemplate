/**
 * \file pou1.cpp
 * \brief Файл пользовательской POU
 * \details
 * Файл должен включать строки
 * \code{.cpp}
 * #include "pou_manager.h"
 * using namespace plc;
 * \endcode
 * для доступа ко всем функциям программируемого контроллера
 */

#include "pou_manager.h"

using namespace plc;


void POU1()
{
  static bool init = false;
  if (!init)
  {
    init = true;
    // Инициализация переменных данной POU
    // Код в этом блоке выполнится 1 раз
    
  }

  ModbusMaster modbus_master ({
    // Номер COM-порта
    .com              = COM1,
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
  

}
