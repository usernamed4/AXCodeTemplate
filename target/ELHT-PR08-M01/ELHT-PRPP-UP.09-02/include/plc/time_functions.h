/**
 * \file plc/time_functions.h
 * \brief Функции работы со временем
 * \ingroup plc_timeFunctions
 */

#pragma once

#include "plc/base.h"
#include "time_menu/time_struct.h"
#include "etl/string.h"

namespace plc
{
  /**
   * \addtogroup plc_timeFunctions
   * \{
   */

  //! Тип данных времени
  using time_type = time_menu::time_type;
  //! Псевдоним используемой структуры времени и даты
  using TimeStruct = time_menu::TimeStruct;

  //! Блок функций RTC (часов реального времени)
  class RTC_Block
  {  
  public:
    //! Возвращает текущие время и дату
    static TimeStruct getTimeStruct();

    //! Возвращает текущие секунды
    static int16_t getSeconds();

    //! Возвращает текущие минуты
    static int16_t getMinutes();

    //! Возвращает текущие часы
    static int16_t getHours();

    //! Возвращает текущий день (число)
    static int16_t getDate();

    //! Возвращает текущий месяц
    static int16_t getMonth();

    //! Возвращает текущий год
    static int16_t getYear();

    //! Возвращает текущий день недели
    static int16_t getDay();

    //! Устанавливает текущие секунды
    static void setSeconds(int16_t seconds);

    //! Устанавливает текущие минуты
    static void setMinutes(int16_t minutes);

    //! Устанавливает текущие часы
    static void setHours(int16_t hours);

    //! Устанавливает текущий день (число)
    static void setDate(int16_t date);

    //! Устанавливает текущий месяц
    static void setMonth(int16_t month);

    //! Устанавливает текущий год
    static void setYear(int16_t year);
    
    //! Устанавливает текущие время и дату через структуру времени
    static void setTime(const TimeStruct& time_struct);

    //! Устанавливает текущие время и дату через переменные времени
    static void setTime(
      int16_t seconds,
      int16_t minutes,
      int16_t hours,
      int16_t date,
      int16_t month,
      int16_t year
    );

    /**
     * \brief Возвращает текущие время и дату в формате строки
     * \details 
     * Формат строки: ЧЧ:ММ:СС ДД.ММ.ГГГГ
     * \return Строка длиной 20 символов
     */
    static etl::string<20> getTimeString();

  private:
    static bool __setTime(const TimeStruct&);
  };


  //! Возвращает время после старта контроллера, миллисекунды
  uint32_t GetSysTicks();


  //! Блок функций RTC (часов реального времени)
  extern RTC_Block RTC;

  /**
   * \}
   * \noop addtogroup plc_timeFunctions
   */
}