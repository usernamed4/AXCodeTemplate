/**
 * \file freertos/include/elh/access_port.h
 * \brief Управление доступом при использовании FreeRTOS
 */

#ifndef COMMON_ACCESS_PORT_H
#define COMMON_ACCESS_PORT_H

#include "elh/access.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"


namespace elh
{
  /**
   * \brief Реализация критических секций через FreeRTOS
   * \ingroup common_access_criticalSection
   */
  class access : public iaccess<access>
  {
  private:
    static inline volatile UBaseType_t isr_mask{};
  public:
    static void enter_critical_Impl();
    static void exit_critical_Impl();
    static void enter_critical_isr_Impl();
    static void exit_critical_isr_Impl();
  };


  /**
   * \brief Реализация мьютекса через FreeRTOS
   * \ingroup common_access_mutex
   */
  class mutex : public imutex<mutex>
  {
    using InterfaceClass = imutex<mutex>;
    friend InterfaceClass;

    QueueHandle_t _mutex{};
    StaticSemaphore_t _mutex_storage{};

    void init_Impl();
    bool lock_Impl(unsigned int timeout);
    bool unlock_Impl();
  };
}

#endif /* COMMON_ACCESS_PORT_H */
