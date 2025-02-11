/**
 * \file 02_dinput_doutput.cpp
 * \brief Пример работы с встроенными дискретными входами и выходами
 * \details
 * Встроенный дискретный выход DO1 повторяет состояние встроенного дискретного
 * входа DI1.
 * 
 * 
 * Для запуска примера скопируйте код в src/pou1.cpp.
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 10, 1000);  // Период вызова 10 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Дискретный вход №1 модуля CPU (DI1)
DiscreteInput di1(cpu_io, CPU_DINPUT1);
//! Дискретный выход №1 модуля CPU (DO1)
DiscreteOutput do1(cpu_io, CPU_DOUTPUT1);


void POU1()
{
  // Код, выполняемый циклически:
  do1 = di1;
}
