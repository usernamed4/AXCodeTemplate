/**
 * \file plc/pou_scheduler.h
 * \brief Функции и настройки планировщика POU
 * \ingroup plc_scheduler
 */

#pragma once

#include "plc/base.h"

/**
 * \brief Функция для добавления пользовательских POU и настройки WatchDog
 * \ingroup plc_scheduler
 */
void PLC_MainSetup();

namespace plc
{
  /**
   * \addtogroup plc_scheduler
   * \{
   */

  /**
   * \brief Режимы сторожевого таймера
   */
  enum class WATCHDOG_MODE
  {
    SERVICE, //!< Перезагружает ПЛК в сервисный режим
    RELOAD,  //!< Перезагружает ПЛК
    STOP,    //!< Останавливает ПЛК (режим STOP)
    SIZE     //!< Кол-во вариантов настройки таймера
  };

  /**
   * \brief Режимы обработчика HardFault
   */
  enum class HARDFAULT_MODE
  {
    SERVICE, //!< Режим остановки ПЛК при возникновении HardFault
    RELOAD,  //!< Режим перезагрузки ПЛК при возникновении HardFault
    SIZE     //!< Кол-во вариантов настройки таймера
  };


  //! Настройки планировщика POU
  struct SchedulerConfig
  {
    //! Режим работы сторожевого таймера POU
    static const WATCHDOG_MODE  watchdog_mode;

    //! Режим обработки HardFault
    static const HARDFAULT_MODE hardfault_mode;
  };


  /**
   * \brief Функция добавления POU в список для циклического выполнения
   *
   * \param pou_ptr Указатель на функцию с кодом POU (имя функции)
   * \param cycle_time Время цикла. Минимальное время между вызовами POU
   * \param watchdog Время сторожевого таймера в милисекундах. Минимум 10 мс
   */
  void AddPOU(void (*pou_ptr)(void), uint32_t cycle_time, uint32_t watchdog = 0);

  /**
   * \brief Получить среднее время выполнения POU
   *
   * \details Используется упрощенный фильтр Калмана: x[i]=(x[i-1]*9+NEW_VALUE)/10
   *
   * \param pou_ptr Имя POU
   * \return uint32_t Время в микросекундах
   */
  uint32_t GetCycleTimeAVG(void (*pou_ptr)(void));

  /**
   * \brief Получить минимальное время выполнения POU
   *
   * \param pou_ptr Имя POU
   * \return uint32_t Время в микросекундах
   */
  uint32_t GetCycleTimeMIN(void (*pou_ptr)(void));

  /**
   * \brief Получить максимальное время выполнения POU
   *
   * \param pou_ptr Имя POU
   * \return uint32_t Время в микросекундах
   */
  uint32_t GetCycleTimeMAX(void (*pou_ptr)(void));

  /**
   * \brief Получить время выполнения POU в крайний раз
   *
   * \param pou_ptr Имя POU
   * \return uint32_t Время в микросекундах
   */
  uint32_t GetCycleTimeLAST(void (*pou_ptr)(void));

  /**
   * \brief Возвращает сколько раз POU выполнилась со старта Alpha-X CPU
   *
   * \param pou_ptr Имя POU
   * \return uint32_t Количество
   */
  uint32_t GetCyclesCount(void (*pou_ptr)(void));

  /**
   * \}
   * \noop plc_scheduler
   */
}
