/**
 * \file 03_serialport_write_read.cpp
 * \brief Пример отправки и получения сообщений через SerialPortWriteRead
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * Пример будет отправлять строку "Input password: " и ждать ввода 
 * пользовательского текста в течение 30 секунд. Прием текста завершается 
 * при получении символа переноса строки "\\n". При следующем выполнении блока
 * в примере сначала будет отправлен полученный текст.
 * 
 * ВНИМАНИЕ! Во время ввода не будет отображаться введенный текст
 *
 * Пример программы реализован на COM2 со следующими параметрами: 
 * Скорость - 19200 бит/с
 * Количество стоп-бит - 1
 * Контроль четности - отсутствует
 * 
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите требуемые настройки связи в serial_port
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Буфер приема посылки
etl::array<char, 256> read_array;
//! Буфер отправки посылки
etl::string<256> send_array;
//! Поток заполнения буфера отправки
etl::string_stream stream{send_array};

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

//! Функциональный блок отправки посылки и ожидания ответа
SerialPortWriteRead write_read  ({
  // ФБ прямого доступа к порту
  .port             = &serial_port,
  // Буфер отправки посылки
  .send_buffer      = send_array,
  // Буфер приема ответа
  .read_buffer      = read_array,
  // Условие завершения приема
  .stop_condition   = ReadStopConditionSymbol('\n'),
  // Время ожидания ответа (мс)
  .response_timeout = 30000,
  // Время паузы между отправкой и приемом (мс)
  .response_delay   = 0
});


void POU1()
{
  if (write_read.done()) // Успешное выполнение запроса
  {
    write_read.execute = false;
    // Перезапись буфера отправки
    stream.str("\r\n");
    // Вывод полученной строки
    stream << "Password received: " << write_read.string_received();
  }
  else if (write_read.error()) // Ошибка выполнения запроса
  {
    write_read.execute = false;
    // Перезапись буфера отправки
    stream.str("\r\n"); 
    // Вывод полученной строки, если есть
    if(write_read.bytes_received() != 0){
      stream << "Password received: " << write_read.string_received() << "\r\n";
    }else{
      stream << "No password received";
    }
    // Проверка номера ошибки
    switch(write_read.error_id())
    {
      case FB_ERROR_CODE_TIMEOUT: // Прошло 30 секунд
      {
        if(write_read.bytes_received() != 0)
        {
          stream << "Warning: No stop character was received\r\n";
        }
        break;
      }
      case SERIAL_ERROR_CODE_RECEIVE_CONDITION_FAIL:
      {
        stream << "Receive buffer is full";
        break;
      }
      default:
      {
        stream << "write_read error id" << write_read.error_id();
        break;
      }
    }
  }
  else if(write_read.idle())
  {
    write_read.execute = true;
    // Вывод строки ожидания ввода
    stream << "\r\nInput password: ";
    // Задание отправляемой строки
    write_read.send_buffer = stream.str();
  }
  write_read(); // Выполнение запроса
}
