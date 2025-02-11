/**
 * \file 02_serialport_write.cpp
 * \brief Пример отправки сообщений через SeralPortWrite
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * Пример будет отправлять строку "example" с номером сообщения на порт COM2.
 * Для приема строки через USB в функциональном блоке serial_port можно
 * поменять COM2 на COM_USB (см. предупреждение ниже).
 * 
 * ВНИМАНИЕ! При использовании COM_USB в serial_port загрузка программы
 * по USB будет доступна только при сбросе питания ПЛК с нажатой кнопкой
 * RUN/STOP. 
 * 
 * Пример реализован на COM2 со следующими параметрами: 
 * Скорость - 19200 бит/с
 * Количество стоп-бит - 1
 * Контроль четности - COM_PARITY_NONE
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите требуемые настройки связи в serial_port
 *   - Задайте свою строку в переменной send_array
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 100, 1000);  // Период вызова 100 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Буфер для записи
etl::string<32> send_string;

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

//! Функциональный блок отправки посылки
SerialPortWrite serial_port_write ({
  // ФБ прямого доступа к порту
  .port        = &serial_port,
  // Буфер отправки посылки
  .send_buffer = {}
});

//! Номер отправляемой посылки
size_t number = 0;


void POU1()
{
  serial_port_write.execute = true; // Активация запроса
  if (serial_port_write.done())     // Успешное выполнение запроса
  {
    serial_port_write.execute = false;
  }
  else if (serial_port_write.error()) // Ошибка выполнения запроса
  {
    serial_port_write.execute = false;
    print_debug("SerialPortWrite error id%i\n", serial_port_write.error_id());
  }
  else if (serial_port_write.idle()) // Запрос не выполняется
  {
    ++number;
    send_string.clear();
    etl::string_stream stream{send_string};
    stream << "example " << number << "\r\n";
    serial_port_write.send_buffer = send_string;
  }
  serial_port_write(); // Выполнение запроса
}
