/**
 * \file elh/timers_port.h
 * \brief Реализация таймеров для FreeRTOS
 * \ingroup common_timers_freertos
 */

#pragma once

#include "elh/timers.h"
#include "FreeRTOS.h"
#include "task.h"

namespace elh
{
  /**
   * \addtogroup common_timers_freertos
   * \{
   */

  /**
   * \brief Реализация миллисекундного таймера таймаута для FreeRTOS
   * \details
   * Использует системный счетчик FreeRTOS как системное время таймера.
   */
  class TimeoutFreeRTOS : public iTimeout<TimeoutFreeRTOS>
  {
  public:
    TimeoutFreeRTOS(uint32_t timeout)
    : iTimeout{timeout}
    {
    }

  protected:
    /**
     * \brief Возвращает системное время таймера таймаута FreeRTOS
     * \details
     * Если ядро не запущено на момент вызова функции, это ошибка и будет 
     * увеличивать счетчик, т.к. время считаться не будет.
     */
    uint32_t __getCurrentCounter_Impl()
    {
      if(xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED){
        return _last_call_counter + 1;
      }else{
        return xTaskGetTickCount();
      }
    }

    friend iTimeout<TimeoutFreeRTOS>;
  };

  /**
   * \brief Старое название FreeRTOS-ных таймеров
   * \deprecated
   * \details
   * Оставлено для совместимости, может быть удалено позднее при исправлении
   * всех зависимостей
   */
  using TimeoutMs = TimeoutFreeRTOS;

  /**
   * \}
   * \noop common_timers_freertos
   */
}
