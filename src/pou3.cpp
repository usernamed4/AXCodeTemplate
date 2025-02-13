/**
 * \file pou3.cpp
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


void POU3()
{
  static bool init = false;
  if (!init)
  {
    init = true;
    // Инициализация переменных данной POU
    // Код в этом блоке выполнится 1 раз

  }

  // Код, выполняемый циклически:
  for(auto i = 0; i<=10; i++)
  {
    print_debug("%i - iterations\r\n", i);
  }
}
