/**
 * \file elh/access.h
 * \brief Библиотека управления доступом
 * \details
 * - Критические секции
 * - Мьютексы
 */


#ifndef COMMON_ACCESS_H
#define COMMON_ACCESS_H

namespace elh
{
  /**
   * \brief Интерфейс критической секции
   * \ingroup common_access_criticalSection
   */
  template<typename IMPL>
  class iaccess
  {
  public:
    static void enter_critical();
    static void exit_critical();
    static void enter_critical_isr();
    static void exit_critical_isr();
  };


  /**
   * \brief Интерфейс мьютекса
   * \ingroup common_access_mutex
   */
  template<typename IMPL>
  class imutex
  {
  public:
    void init();
    bool lock(unsigned int timeout);
    bool unlock();
  private:
    IMPL* __impl() { return static_cast<IMPL*>(this); }
  };


  /**
   * \brief Вход в критическую секцию
   */
  template<typename IMPL>
  void iaccess<IMPL>::enter_critical()
  {
    return IMPL::enter_critical_Impl();
  }


  /**
   * \brief Выход из критической секции
   */
  template<typename IMPL>
  void iaccess<IMPL>::exit_critical()
  {
    return IMPL::exit_critical_Impl();
  }


  /**
   * \brief Вход в критическую секцию в прерывании
   */
  template<typename IMPL>
  void iaccess<IMPL>::enter_critical_isr()
  {
    return IMPL::enter_critical_isr_Impl();
  }


  /**
   * \brief Выход из критической секции в прерывании
   */
  template<typename IMPL>
  void iaccess<IMPL>::exit_critical_isr()
  {
    return IMPL::exit_critical_isr_Impl();
  }


  /**
   * \brief Инициализация мьютекса
   */
  template<typename IMPL>
  void imutex<IMPL>::init()
  {
    return __impl()->init_Impl();
  }


  /**
   * \brief Блокирует доступ к ресурсу
   * \param timeout Время ожидания занятого ресурса
   * \return true если получен доступ к ресурсу
   * \return false если вышел таймаут
   */
  template<typename IMPL>
  bool imutex<IMPL>::lock(unsigned int timeout)
  {
    return __impl()->lock_Impl(timeout);
  }


  /**
   * \brief Отпускает доступ к ресурсу
   */
  template<typename IMPL>
  bool imutex<IMPL>::unlock()
  {
    return __impl()->unlock_Impl();
  }
}

#include "elh/access_port.h"

#endif /* COMMON_ACCESS_H */
