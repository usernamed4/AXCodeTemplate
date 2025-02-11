/**
 * \file 01_modbus_slave.cpp
 * \brief Пример настройки slave-регистров ПЛК
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * 
 * Пример настройки Slave-регистров ПЛК на COM3.
 * Регистры можно опросить на стандартных настройках связи ПЛК
 * (см. modbus_slave).
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - При необходимости:
 *   - Установите настройки связи в modbus_slave
 *   - Запишите собственные регистры в POU1() через методы reg_bug.write...()
 *     и coil_buf.write...()
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 100, 1000);  // Период вызова 100 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;


//! Таблица для регистров Holding register и Input Registers на 12 элементов
ModbusBuffer<12> reg_buf;
//! Таблица для регистров Coils и Discrete Inputs на 10 элементов
ModbusCoilBuffer<10> coil_buf;

//! Функциональный блок Modbus Slave на COM3
ModbusSlave modbus_slave ({
  // Номер COM-порта
  .com               = COM3,
  // Таблица Holding Registers
  .holding_registers = &reg_buf,
  // Таблица Input Registers
  .input_registers   = &reg_buf,
  // Таблица Coils
  .coils             = &coil_buf,
  // Таблица Discrete Inputs
  .discrete_inputs   = &coil_buf,
  // Адрес Slave в сети Modbus
  .address           = 1,
  // Скорость обмена, бит/с
  .baudrate          = 115200,
  // Четность
  .parity            = COM_PARITY_NONE,
  // Кол-во стоп-битов
  .stop_bits         = 1,
  // Пауза перед отправкой ответа (мс)
  .poll_delay        = 0
});


void POU1()
{
  // Запись регистров типа Holding Registers и Input Registers
  // Запись в регистр с адресом 0 значения false
  reg_buf.writeBool(0, false);
  // Запись символа A в регистр с адресом 1
  reg_buf.writeChar( 1, 'A');
  // Запись двухбайтного значения 12345 в регистр с адресом 2
  reg_buf.writeShort(2, 12435);
  // Запись числа типа long в регистры 3 и 4.
  reg_buf.writeLong(3, 123456789);
  // Запись числа с плавающей точкой в регистры 5 и 6
  reg_buf.writeFloat(5, 1.2345);

  // Запись регистров типа Coils
  coil_buf.writeCoil(0, true);  // Устанавливает 1 для Coil по адресу 0
  coil_buf.writeCoil(1, false); // Устанавливает 0 для Coil по адресу 1
  coil_buf.writeCoilsBitmask(2, 8, 0b10101010); // запись Coil по маске 10101010

  // Чтение из регистров типа Holding register и Input Registers
  // Чтение значения типа uint16_t из регистра с адресом 7
  uint16_t my_uint  = reg_buf.readUint16(7);
  // Чтение значения типа int32_t из регистров 8 и 9
  int32_t  my_int   = reg_buf.readInt32(8);
  // Чтение значения типа float из регистров 10 и 11.
  float    my_float = reg_buf.readFloat(10);

  // Вывод переменных в терминал. Float выводится с 3 знаками после запятой
  etl::string<128> str;
  etl::string_stream stream{str};
  stream << "[" << my_uint << "][" << my_int << "]["
         << etl::setprecision(3) << my_float << "]\r\n";
  print_debug(str);
}
