/**
 * \file 01_doutput_blink.cpp
 * \brief Пример работы с встроенным дискретным выходом
 * \details
 * При запуске POU происходит цикличное изменение состояния встроенного 
 * дискретного выхода DO1 (мигание лампочкой).
 * 
 * 
 * Для запуска примера скопируйте код в src/pou1.cpp.
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 500, 1000);  // Период вызова 500 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

// Дискретный выход модуля CPU (DO1)
DiscreteOutput do1(cpu_io, CPU_DOUTPUT1);


void POU1()
{
  // Код, выполняемый циклически:
  do1 = !do1;
}
