/**
 * \file 01_serial_port_read.cpp
 * \brief Пример получения сообщений через SerialPortRead
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * Пример будет пересылать символы, получаемые по COM-порту, в терминал отладки,
 * подключенный к USB. Пересылка начнется спустя 10 мс после приема последнего
 * символа.
 * Терминал отладки настроен на отправку только пользовательских отладочных
 * сообщений (события и ошибки ПЛК отображаться не будет).
 * Для просмотра событий и ошибок ПЛК в примере создан дополнительный порт
 * отладки на COM3 со стандартными настройками связи (115200, 1 стоп-бит,
 * без четности)
 *
 * Пример реализован на COM2 со следующими параметрами: 
 * - Скорость - 19200 бит/с
 * - Количество стоп-бит - 1
 * - Контроль четности - отсутствует
 * 
 * Существует несколько вариантов завершения приема посылки: 
 * - ReadStopConditionPause(10) - по указанной паузе в миллисекундах после
 * последнего приема данных (10 мс)
 * - ReadStopConditionSymbol('\\n') - по приему указанного символа ('\\n')
 * - ReadStopConditionString("stop") - по приему указанной строки ("stop")
 * - ReadStopConditionFull() - при заполнении буфера приема
 * 
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите требуемые настройки связи в serial_port
 *   - Выберите COM-порт и настройки связи для пересылки посылок в терминал
 *     в echo_debug_port
 *   - Установите настройки связи дополнительного порта отладки в debug_port
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Буфер приема посылки
etl::array<char, 256> read_array;

//! Функциональный блок прямого доступа к порту COM2
SerialPort serial_port ({
  // Номер COM-порта
  .com       = COM2,
  // Скорость обмена, бит/с
  .baudrate  = 19200,
  // Четность
  .parity    = COM_PARITY_NONE,
  // Кол-во стоп-битов
  .stop_bits = 1
});

//! Функциональный блок приема посылки
SerialPortRead serial_port_read ({
  // ФБ прямого доступа к порту
  .port             = &serial_port,
  // Буфер приема посылки
  .read_buffer      = read_array,
  // Условие завершения приема
  .stop_condition   = ReadStopConditionPause(10),
  // Время ожидания ответа (мс). "0" - максимальное время ожидания
  .response_timeout = 0,
  // Время паузы перед началом приема (мс)
  .response_delay   = 0
});

//! Функциональный блок порта отладки на COM_USB
DebugPort echo_debug_port ({
  // Номер COM-порта
  .com            = COM_USB,
  // Скорость обмена, бит/с
  .baudrate       = 115200,
  // Четность
  .parity         = COM_PARITY_NONE,
  // Кол-во стоп-битов
  .stop_bits      = 1,
  // Отправлять отладочные сообщения
  .debug_messages = true,
  // Отправлять сообщения событий
  .event_messages = false,
  // Отправлять сообщения ошибок
  .error_messages = false
});

//! Резервный порт отладки для просмотра ошибок и событий ПЛК на COM3
DebugPort<1024> debug_port ({
  // Номер COM-порта
  .com            = COM3,
  // Скорость обмена, бит/с
  .baudrate       = 115200,
  // Четность
  .parity         = COM_PARITY_NONE,
  // Кол-во стоп-битов
  .stop_bits      = 1,
  // Отправлять отладочные сообщения
  .debug_messages = true,
  // Отправлять сообщения событий
  .event_messages = true,
  // Отправлять сообщения ошибок
  .error_messages = true
});


void POU1()
{
  serial_port_read.execute = true; // Активация запроса
  if (serial_port_read.done())     // Успешное выполнение запроса
  {
    serial_port_read.execute = false;
    print_debug(serial_port_read.string_received());
  }
  else if (serial_port_read.error()) // Ошибка выполнения запроса
  {
    serial_port_read.execute = false;
    print_debug("\nSerialPortRead error id%i\n", serial_port_read.error_id());
  }
  serial_port_read(); // Выполнение запроса
}
