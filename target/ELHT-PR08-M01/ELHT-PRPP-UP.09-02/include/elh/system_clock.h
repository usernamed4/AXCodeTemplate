/**
 * \file elh/system_clock.h
 * \brief Системное время
 */

#pragma once

#include <cstdint>


namespace elh
{
  /**
   * \brief Интерфейс общих функций времени
   * \tparam IMPL Класс реализации интерфейса
   */
  template<typename IMPL>
  class isystem_clock
  {
  public:
    static uint32_t getMs();
  };


  /**
   * \brief Возвращает время после старта системы в миллисекундах
   */
  template<typename IMPL>
  uint32_t isystem_clock<IMPL>::getMs()
  {
    return IMPL::getMs_Impl();
  }
}

#include "elh/system_clock_port.h"
