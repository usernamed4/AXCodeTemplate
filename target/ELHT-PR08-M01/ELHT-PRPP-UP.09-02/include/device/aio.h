/**
 * \file device/aio.h
 * \brief Аналоговые входы и выходы устройств
 */

#pragma once

#include <cstdint>


namespace device
{
  /**
   * \brief Базовый класс аналоговых входов/выходов
   */
  class AnalogBase
  {
  public:
    //! Возвращает значение аналогового входа/выхода
    float get() const
    {
      return _value;
    }

    //! Устанавливает значение аналогового входа/выхода
    bool set(float value)
    {
      _value = value;
      return true;
    }


    /**
     * \brief Возвращает последнее время записи выхода
     * \details
     * Время должно задаваться методом \ref setLastUpdateTime()
     */
    uint32_t getLastUpdateTime() const
    {
      return _last_update_time;
    }


    /**
     * \brief Устанавливает время записи выхода
     * \details
     * Вызывать в функциях обработки успешной записи состояния выхода
     * в целевое устройство
     */
    void setLastUpdateTime(uint32_t time)
    {
      _last_update_time = time;
    }


    /**
     * \brief Включает/выключает опрос аналогового входа/выхода
     * \param state Вкл/выкл аналогового выхода (по умолчанию - вкл.)
     */
    void enable(bool state=true)
    {
      _enabled = state;
    }


    /**
     * \brief Выключает опрос аналогового входа/выхода
     */
    void disable()
    {
      enable(false);
    }

    /**
     * \brief Возвращает состояние опроса аналогового входа/выхода
     * \retval true Опрос включен
     * \retval false Опрос выключен
     */
    bool isEnabled() const
    {
      return _enabled;
    }

  protected:
    float _value;
    uint32_t _last_update_time{};
    bool _enabled{true};

    constexpr AnalogBase(float value=0)
    : _value{value}
    {
    }
  };


  /**
   * \brief Аналоговый вход устройства
   */
  class AInput : public AnalogBase
  {

  };

  /**
   * \brief Аналоговый выход устройства
   */
  class AOutput : public AnalogBase
  {

  };
}
