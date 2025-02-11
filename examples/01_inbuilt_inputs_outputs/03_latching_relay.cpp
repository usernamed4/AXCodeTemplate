/**
 * \file 03_latching_relay.cpp
 * \brief Пример работы с встроенными дискретными входами\выходами
 * \details
 * По фронту сигнала DI1 происходит последовательное изменение состояния выходов
 * ПЛК в следующей последовательности:
 * - DO1 = false, DO2 = false
 * - DO1 = true,  DO2 = false
 * - DO1 = false, DO2 = true
 * - DO1 = true,  DO2 = true
 * 
 * 
 * Для запуска примера скопируйте код в src/pou1.cpp.
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Дискретный вход DI1
DiscreteInput di1 (cpu_io, CPU_DINPUT1);
//! Дискретный выход DO1
DiscreteOutput do1 (cpu_io, CPU_DOUTPUT1);
//! Дискретный выход DO2
DiscreteOutput do2 (cpu_io, CPU_DOUTPUT2);
//! Переменная для хранения текущего состояния
int8_t state = 0;


void POU1()
{
  // Увеличение переменной состояния по фронту сигнала DI1
  if (di1.front()) state++;

  // Сброс переменной состояния при числовом значении больше 3
  if (state > 3) state = 0;

  // Включение-выключение выходов в зависимости от state
  switch (state)
  {
    case 0:
    {
      do1 = false;
      do2 = false; 
      break;
    }
    case 1:
    {
      do1 = true;
      do2 = false;
      break;
    }
    case 2:
    {
      do1 = false;
      do2 = true;
      break;
    }
    case 3:
    {
      do1 = true;
      do2 = true;
      break;
    }
    default:
    {
      state = 0;
      break;
    }
  }
}
