/**
 * \file pou_manager.cpp
 * \brief Файл настройки пользовательских POU
 * \details
 * Задает пользовательские настройки системы и параметры работы POU
 */
#include "pou_manager.h"

using namespace plc;


// НАСТРОЙКИ ПЛАНИРОВЩИКА
// Реакция системы на срабатывание Watchdog планировщика
const WATCHDOG_MODE  SchedulerConfig::watchdog_mode  {WATCHDOG_MODE::SERVICE};
// Реакция системы на HardFault
const HARDFAULT_MODE SchedulerConfig::hardfault_mode {HARDFAULT_MODE::SERVICE};
// Период сохранения retain-переменных
uint8_t RetainConfig::save_time {10};


void PLC_MainSetup()
{
  AddPOU(POU1, 100, 1000);  // Цикл 100 мс функции POU1, сторожевой таймер 1 сек

  // Раскомментируйте строки ниже для включения POU файлов pou2.cpp и pou3.cpp
  // AddPOU(POU2, 1000, 1000); // Цикл 1 сек функции POU2, сторожевой таймер 1 сек
  // AddPOU(POU3, 2000, 1000); // Цикл 2 сек функции POU3, сторожевой таймер 1 сек
}
