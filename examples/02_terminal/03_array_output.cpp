/**
 * \file 03_array_output.cpp
 * \brief Пример вывода массива в терминал по команде "u print"
 * \details
 * При получении пользовательской команды "u print" из терминала
 * происходит вывод значений массива в терминал:
 * [4, 8, 15, 16, 23, 42];
 * 
 * 
 * Для запуска примера скопируйте код в src/pou1.cpp.
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Массив с данными
etl::array<int16_t, 6> my_array{4, 8, 15, 16, 23, 42};
//! Строка для получения команды пользователя
etl::string<20> t_command;


/**
 * \brief Функция для вывода массива типа int16_t в терминал
 * \param arr Класс указателей на начало и конец массива
*/
void print_array(etl::span<int16_t> arr)
{
  print_debug("[");
  for (size_t i = 0; i < (arr.size() - 1); i++)
  {
    print_debug("%d, ", arr[i]);
  }
  // Вывод последнего элемента массива без запятой
  print_debug("%d]\r\n", arr.back());
}


void POU1()
{
  // Получение строки из буфера обмена
  scan_debug(t_command);

  // Проверка соответствия команды
  if (t_command == "print")
  {
    // Вызов функции вывода
    print_array(my_array);
  }
}
