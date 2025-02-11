/**
 * \file 02_delay_using_ton.cpp
 * \brief Пример задержки включения с помощью функционального блока TON
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * 
 * Выход DO1 срабатывает через время .pt после появления сигнала на входе DI1.
 * Если в терминал отправляется время, оставшееся до конца отсчета таймера.
 * 
 * 
 * Для запуска примера скопируйте код в src/pou1.cpp.
 * Настройки POU в функции PLC_MainSetup() в файле pou_manager.cpp:
 * AddPOU(POU1, 100, 1000);  // Период вызова 100 мс, сторожевой таймер 1 сек
 * 
 */

#include "pou_manager.h"

using namespace plc;

//! Дискретный вход модуля CPU (DI1)
DiscreteInput di1(cpu_io, CPU_DINPUT1);
//! Дискретный выход модуля CPU (DO1)
DiscreteOutput do1(cpu_io, CPU_DOUTPUT1);
//! Экземпляр функционального блока TON с уставкой времени 3000 мс
TON ton_timer({.pt = 3000});


void POU1()
{
  // Вход запуска таймера - дискретный вход DI1
  ton_timer.en = di1;
  // Выход таймера - дискретный выход DO1
  do1 = ton_timer.q();
  // Проверка на условия работы таймера
  bool ton_running = ton_timer.en & (ton_timer.q() == false);
  // Вызов ФБ
  ton_timer();
  if(ton_running)
  {
    // Отправка в терминал времени, оставшегося до конца отсчета
    print_debug("%d\r\n", ton_timer.rt());
  }
}
