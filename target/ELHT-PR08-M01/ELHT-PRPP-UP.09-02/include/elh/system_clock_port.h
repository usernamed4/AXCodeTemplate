/**
 * \file elh/system_clock_port.h
 * \brief Реализация функций системного времени для порта на FreeRTOS
 */

#pragma once 

#include "elh/system_clock.h"


namespace elh
{
  /**
   * \brief Реализация системного времени для FreeRTOS
   */
  class system_clock : public isystem_clock<system_clock>
  {
    using base_class = isystem_clock<system_clock>;
    friend base_class;

    /**
     * \brief Реализация метода \ref getMs() для FreeRTOS
     */
    static uint32_t getMs_Impl();
  };
}
