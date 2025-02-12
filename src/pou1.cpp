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

  DiscreteOutputArray<2> DO (cpu_io);

  ModbusBuffer<10> buffer;

  ModbusSlave slave_com2 ({
      .com = COM2,
      .holding_registers = &buffer,
      .input_registers   = &buffer,
      .address   = 1,
      .baudrate  = 115200,
      .parity    = COM_PARITY_NONE,
      .stop_bits = 1
      });

  uint32_t i = 0;

  void delay(uint32_t ms)
  {
    uint32_t set_t = GetSysTicks();
    while ((GetSysTicks() - set_t) < ms){print_event("delay...(%d)\r\n", GetSysTicks() - set_t);}
  }

void POU1()
{
  static bool init = false;
  if (!init)
  {
    init = true;
    // Инициализация переменных данной POU
    // Код в этом блоке выполнится 1 раз
    DO[0] = true;
    DO[1] = false;
  }

  // Код, выполняемый циклически:
  print_debug("Baudrate %d\t Address %d\n\r", slave_com2.baudrate, slave_com2.address);
  print_debug("GetSysTicks %d.%d s\n\r", GetSysTicks()/1000, GetSysTicks()%1000);
  print_debug(RTC.getTimeString());
  print_debug("\r\n\r\n");
  
  DO[1] = DO[0].value();
  DO[0] = !DO[1].value();
  
}
