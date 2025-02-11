/**
 * \file kernel/kernel_resource.h
 * \brief Ресурсы общего доступа ядра
 * \ingroup kernel_os_resource
 */

#pragma once

#include "kernel/kernel_base.h"

namespace kernel
{
  /**
   * \addtogroup kernel_os_resource
   * \{
   */

  /**
   * \brief Ресурс с защитой доступа через мьютекс
   * \tparam T Тип защищаемого объекта
   */
  template<typename T>
  struct resource_t : public ETL_OR_STD::pair<T, elh::mutex>
  {
    //! Тип базового класса
    using base = ETL_OR_STD::pair<T, elh::mutex>;

    /**
     * \brief Конструктор ресурса по аргументам конструктора объекта ресурса
     * \tparam ARGS Типы аргументов конструктора объекта ресурса
     * \param args Аргументы конструктора объекта ресурса
     */
    template<typename... ARGS>
    constexpr resource_t(ARGS... args)
    : base{T{args...}, {}}
    {
    }

    /**
     * \brief Копирующий конструктор ресурса
     * \param resource Хранимый объект в ресурсе
     */
    constexpr resource_t(const T& resource)
    : base{resource, {}}
    {
    }

    //! Возвращает ссылку на ресурс
    constexpr T& resource(){ return base::first; }

    //! Возвращает ссылку на мьютекс
    constexpr elh::mutex& mutex(){ return base::second; }

    //! Возвращает ссылку на ресурс
    constexpr T& operator()(){ return resource(); }

    //! Возвращает ссылку на ресурс
    constexpr T& operator*() { return resource(); }

    //! Возвращает указатель на ресурс
    constexpr T* operator->() { return &resource(); }

    /**
     * \brief Блокирует ресурс
     * \param timeout Время ожидания занятого ресурса
     * \return true если получен доступ к ресурсу
     * \return false если вышел таймаут
    */
    bool lock(uint32_t timeout){ return mutex().lock(timeout); }

    /**
     * \brief Разблокировывает доступ к ресурсу
     * \details
     * Метод должен вызываться только если ресурс был успешно заблокирован.
     * \retval true Доступ успешно разблокирован
     * \retval false Произошла ошибка выполнения разблокировки
     */
    bool unlock(){ return mutex().unlock(); }
  };

  /**
   * \}
   * \noop kernel_os_resource
   */
}