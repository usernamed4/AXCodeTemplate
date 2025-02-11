/**
 * \file elh/timers.h
 * \brief Таймеры и счетчики
 * \ingroup common_timers
 */

#pragma once

#include <cstdint>

namespace elh
{
  /**
   * \addtogroup common_timers
   * \{
   */

  /**
   * \brief Интерфейс таймера таймаута
   * \tparam IMPL Класс реализации таймера
   */
  template<typename IMPL>
  class iTimeout
  {
  protected:
    uint32_t _last_call_counter; //!< Время последнего вызова таймера таймаута
    uint32_t _passed_counter;    //!< Прошедшее с момента запуска время
    uint32_t _timeout;           //!< Время таймаута
    bool     _stop;              //!< Флаг остановки таймера пользователем

    /**
     * \brief Считывает текущее значение счетчика таймера таймаута
     * \details
     * Метод должен быть реализован в классах таймаута
     */
     uint32_t __getCurrentCounter()
    {
      return static_cast<IMPL*>(this)->__getCurrentCounter_Impl();
    }

    /**
     * \brief Конструктор интерфейса таймера таймаута
     * \param timeout Время таймаута (мс)
     * \details
     * 
     */
    iTimeout(uint32_t timeout)
    : _last_call_counter{},
      _passed_counter{},
      _timeout{},
      _stop{}
    {
      reset(timeout);
    }

  public:
    /**
     * \brief Сбрасывает таймер
     * \param timeout Новое время таймаута, если не равно 0
     * \details
     * Обновляет время запуска таймера на текущее и сбрасывает флаги
     */
    void reset(uint32_t timeout=0)
    {
      _last_call_counter = __getCurrentCounter();
      _passed_counter = 0;
      if(timeout)
      {
        _timeout = timeout;
      }
      _stop = false;
    }


    /**
     * \brief Останавливает таймер таймаута
     */
    void stop()
    {
      _stop = true;
    }


    /**
     * \brief Продолжает счет без перезапуска таймера
     */
    void resume()
    {
      _stop = false;
    }


    /**
     * \brief Возвращает true, если вышло время таймаута
     * \details
     * Метод должен вызываться только после остановки таймера по любой причине
     */
    bool is_timed_out() const
    {
      return !_stop;
    }


    /**
     * \brief Выполняет проверку таймаута
     * \param stop Флаг остановки таймера
     * \details
     * Останавливает таймер по условию stop или если вышло время таймаута.
     * 
     * \note При использовании одного объекта таймера таймаута несколько
     * раз нужно перед вызовом метода обязательно использовать методы
     * \ref resume() или \ref reset() для сброса внутреннего флага остановки
     * таймаута
     * 
     * \retval true Если таймер продолжает считать
     * \retval false Если таймер остановлен или вышло время таймаута
    */
    bool operator()(bool stop={})
    {
      if(stop)
      {
        _stop = true;
      }

      uint32_t new_counter = __getCurrentCounter();
      _passed_counter += static_cast<int32_t>(new_counter - _last_call_counter);
      _last_call_counter = new_counter;

      if(_stop || (_passed_counter >= _timeout)){
        return false;
      }else{
        return true;
      }
    }


    /**
     * \brief Возвращает прошедшее время таймаута
     */
    uint32_t getCounter()
    {
      return _passed_counter;
    }
  };


  /**
   * \brief Таймаут со счетчиком вызовов цикла таймаута
   * \details
   * Работает только с увеличением переменной в цикле, может работать без
   * прерываний и аппаратной периферии, но невозможно точно посчитать
   * время
   */
  class TimeoutCall : public iTimeout<TimeoutCall>
  {
  public:
    TimeoutCall(uint32_t timeout)
    : iTimeout{timeout}
    {
    }

  private:
    /**
     * \brief Увеличивает последний счетчик на 1
     */
    uint32_t __getCurrentCounter_Impl()
    {
      return _last_call_counter + 1;
    }

    friend iTimeout<TimeoutCall>;
  };

  /**
   * \}
   * \noop common_timers
   */
}

#include "elh/timers_port.h"
