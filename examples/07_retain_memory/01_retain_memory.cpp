/**
 * \file 01_volatile_memory.cpp
 * \brief Пример управления энергонезависимой памятью
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00
 * Чтобы включить сохранение данных в EEPROM при загрузке программы пользователя 
 * необходимо в файле tasks.json добавить аргумент "--save-retain". Значение 
 * переменных будет взято из EEPROM и инициализация в коде программы 
 * пользователя будет игнорироваться.
 * 
 * Выводит в терминал значения retain-переменных.
 * По команде "u add" из терминала удваивает значения переменных.
 * После перезагрузки или сброса питания в программе должны отображаться
 * последние записанные значения переменных.
 *  
 * ВНИМАНИЕ! Сохранение происходит с определенным интервалом, задаваемым
 * в файле src/pou_manager.cpp в параметре "RetainConfig::save_time"
 * (по умолчанию раз в 10 секунд).
 * 
 * Значение retain-переменных может отличаться от заданных в примере, если
 * программа была загружена с флагом "--save-retain" в AXCode Loader
 * 
 * Для запуска примера:
 * - Скопируйте код в src/pou1.cpp
 * - Загрузите программу в ПЛК без флага "--save-retain" в AXCode Loader
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 1000, 1000);  // Период вызова 1000 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;


// Переменные разных типов, записанные в энегонезависимую память 
retain uint32_t num1=1;
retain float num2 = 2.54f;
retain int mass[4] = {5, 7, 12, 45};
retain bool logic = true;


void POU1()
{
  // Строка для чтения команды и вывода значений переменных
  etl::string<256> str; 
  // Проверка команды пользователя
  scan_debug(str);
  if(str == "add")
  {
    num1 *= 2;
    num2 *= 2;
    for(size_t i = 0; i < 4; ++i)
    {
      mass[i] *= 2;
    }
    logic = !logic;
  }

  // Вывод значений retain в терминал
  etl::string_stream stream{str};
  str.clear();
  stream << "Retain values:\n" << etl::setprecision(2)
         << "num1 = " << num1 << "\n"
         << "num2 = " << num2 << "\n";
  for(size_t i = 0; i < (sizeof(mass) / sizeof(int)); ++i)
  {
    stream << "mass[" << i << "] = " << mass[i] << "\n";
  }
  stream << etl::boolalpha << "logic = " << logic << "\n\n";
  print_debug(stream.str());
}
