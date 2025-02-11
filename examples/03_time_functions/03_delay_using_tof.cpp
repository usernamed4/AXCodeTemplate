/**
 * \file 03_delay_using_tof.cpp
 * \brief Пример задержки выключения с помощью функционального блока TOF
 * \details
 * Пример программы написан под Alpha-X CPU 01-1 00.
 * 
 * Выход DO1 срабатывает сразу после подачи сигнала на вход DI1.
 * Таймер начинает отсчет после снятия сигнала со входа DI1 и через
 * время .pt выход DO1 отключается.
 * 
 * В терминал отправляется время, оставшееся до конца отсчета таймера.
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
//! Экземпляр функционального блока TOF с уставкой времени 3000 мс
TOF tof_timer;


void POU1()
{
  // Уставка таймера
  tof_timer.pt = 5000;
  // Вход запуска таймера - дискретный вход DI1
  tof_timer.en = di1;
  // Выход таймера - дискретный выход DO1
  do1 = tof_timer.q();
  // Проверка на условия работы таймера
  bool tof_running = (tof_timer.en == false) && tof_timer.q();
  // Вызов ФБ
  tof_timer();
  if(tof_running)
  {
    // Отправка в терминал времени, оставшегося до конца отсчета
    print_debug("%d\r\n", tof_timer.rt());
  }
}
