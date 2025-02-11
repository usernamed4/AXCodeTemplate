/**
 * \file 01_delay_using_systicks.cpp
 * \brief Пример задержки включения с помощью системного таймера
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * 
 * Выход DO1 срабатывает через время delay (мс) после появления
 * сигнала на входе DI1.
 * 
 * 
 * Для запуска примера скопируйте код в src/pou1.cpp.
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Задержка срабатывания в миллисекундах
uint32_t delay = 3'000;
//! Отметка времени предыдущего срабатывания таймера
uint32_t time_stamp = 0;
//! Дискретный вход модуля CPU (DI1)
DiscreteInput di1(cpu_io, CPU_DINPUT1);
//! Дискретный выход модуля CPU (DO1)
DiscreteOutput do1(cpu_io, CPU_DOUTPUT1);


void POU1()
{
  // Отметка времени сдвигается каждый цикл, до тех пор пока DI1 не сработает
  if (!di1)
  {
    time_stamp = GetSysTicks();
  }

  // Включение DO1 происходит только через время delay после срабатывания DI1
  do1 = false;
  if ((GetSysTicks() - time_stamp) >= delay)
  {
    do1 = true;
  }
}
