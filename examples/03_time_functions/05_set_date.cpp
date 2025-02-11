/**
 * \file 05_set_date.cpp
 * \brief Пример задания текущей даты через терминал
 * \details
 * Пользовательские команды пишутся с префиксом "u" в начале. Список возможных
 * команд указан ниже, при этом xx - задаваемое пользователем значение:
 * - Изменения года: u year xxxx
 * - Изменения месяца: u month xx
 * - Изменения числа: u date xx
 * 
 * 
 * Для запуска примера скопируйте код в src/pou1.cpp.
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Строка для хранения полученной команды пользователя
etl::string<20> t_command; 


void POU1()
{
  // Получение строки из буфера обмена
  scan_debug(t_command);

  // Проверка, что строка не пустая
  if(!t_command.empty())
  {
    //Расшифровка строки с помощью функции sscanf
    char str[20];
    int16_t new_value;
    bool command_error = false;
    int result = sscanf(t_command.c_str(),"%s %hd", str, &new_value);
    if(result == 2)
    {
      t_command.assign(str);
      etl::to_lower_case(t_command);
      
      if (t_command == "year")  // Обработка команды на изменение года 
      {
        RTC.setYear(new_value); 
      }
      else if(t_command == "month") // Обработка команды на изменение месяца 
      {
        RTC.setMonth(new_value);
      }
      else if(t_command == "date") // Обработка команды на изменение дня 
      {
        RTC.setDate(new_value);
      }
      else // Неизвестный текстовый аргумент
      {
        command_error = true;
      }
    }
    else // Неправильные аргументы команды
    {
      command_error = true;
    }

    if(command_error)
    {
      print_debug(
        "Wrong command arguments. Usage:\r\n"
        "- u year xxxx - set year to xxxx\r\n"
        "- u month xx - set month to xx\r\n"
        "- u date xx - set date to xx\r\n"
        "\r\n"
      );
    }
  }
  
  // Вывод текущего времени в терминал
  print_debug(RTC.getTimeString());
  print_debug("\r\n");
}
