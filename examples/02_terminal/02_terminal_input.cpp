/**
 * \file 02_terminal_input.cpp
 * \brief Пример управления дискретными выходами при помощи команд из терминила 
 * \details
 * Пример программы написан для Alpha-X CPU 01-1 00.
 * 
 * При получении пользовательской команды из терминала происходит изменение
 * дискретных выходов ПЛК. Формат команды:
 * u @X @V
 * , где u  - префикс пользовательской команды,
 *       @X - указание дискретного выхода (DO1 или DO2),
 *       @V - устанавливаемое значение дискретного выхода (0 - выключить,
 *            1 - включить)
 * 
 * 
 * Для запуска примера скопируйте код в src/pou1.cpp.
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Дискретный выход DO1
DiscreteOutput do1 (cpu_io, CPU_DOUTPUT1);
//! Дискретный выход DO2
DiscreteOutput do2 (cpu_io, CPU_DOUTPUT2);
//! Строка для хранения полученной команды пользователя
etl::string<10> t_command;


void POU1()
{
  // Получение строки из буфера обмена
  scan_debug(t_command);

  // Проверка, что строка не пустая
  if (!t_command.empty())
  {
    //Расшифровка строки с помощью функции sscanf
    char str[10];
    int16_t new_value = 0;
    bool command_error = false;

    int result = sscanf(t_command.c_str(), "%s %hd", str, &new_value);
    if(result == 2) // 2 аргумента преобразованы правильно
    {
      t_command.assign(str);
      etl::to_upper_case(t_command);
      // Установка параметров
      if(t_command == "DO1")
      {
        do1 = new_value; // Присвоение выхода DO1
      }
      else if(t_command == "DO2")
      {
        do2 = new_value; // Присвоение выхода DO@
      }
      else
      {
        command_error = true; // Неизвестный текстовый арумент
      }
    }
    else 
    {
      command_error = true; // Неправильные аргументы команды
    }

    if(command_error == false)
    {
      // Вывод состояния выходов после применения команды
      print_debug("DO1: %d DO2: %d\n", do1.value(), do2.value());
    }
    else
    {
      // Вывод справки по команде
      print_debug(
        "Wrong command arguments. Usage:\n"
        "u @X @Y\n"
        "- @X is \"DO1\" or \"DO2\"\n"
        "- @Y is 0 for false and other for true\n"
      );
    }
  }
}
