/**
 * \file time_menu/base.h
 * \brief Базовые определения библиотеки
 */

#pragma once

#include <cstdint>
#include "date/date.h"

namespace time_menu
{
  /**
   * \addtogroup timeMenu_base
   * \{
   */

  //! Тип данных UNIX-времени
  using time_type = int64_t;
  //! Тип данных календаря (ДД/ММ/ГГ)
  using year_month_day = date::year_month_day;
  //! Тип данных реального времени
  using time_of_day = date::time_of_day<std::chrono::seconds>;

  //! Список месяцев
  enum MONTHS
  {
    JANUARY   = 1,  //!< Январь
    FEBRUARY  = 2,  //!< Февраль
    MARCH     = 3,  //!< Март
    APRIL     = 4,  //!< Апрель
    MAY       = 5,  //!< Май
    JUNE      = 6,  //!< Июнь
    JULY      = 7,  //!< Июль
    AUGUST    = 8,  //!< Август
    SEPTEMBER = 9,  //!< Сентябрь
    OCTOBER   = 10, //!< Октябрь
    NOVEMBER  = 11, //!< Ноябрь
    DECEMBER  = 12  //!< Декабрь
  };

  //! Список дней недели
  enum DAY_OF_WEEK
  {
    MONDAY    = 1, //!< Понедельник
    TUESDAY   = 2, //!< Вторник
    WEDNESDAY = 3, //!< Среда
    THURSDAY  = 4, //!< Четверг
    FRIDAY    = 5, //!< Пятница
    SATURDAY  = 6, //!< Суббота
    SUNDAY    = 7  //!< Воскресенье
  };

  //! Часовые пояса UTC
  enum TIME_ZONE
  {
    TIME_ZONE_UTC_MINUS12   = -1200, //!< Часовой пояс UTC-12
    TIME_ZONE_UTC_MINUS11   = -1100, //!< Часовой пояс UTC-11
    TIME_ZONE_UTC_MINUS10   = -1000, //!< Часовой пояс UTC-10
    TIME_ZONE_UTC_MINUS9_30 = -930,  //!< Часовой пояс UTC-9:30
    TIME_ZONE_UTC_MINUS9    = -900,  //!< Часовой пояс UTC-9
    TIME_ZONE_UTC_MINUS8    = -800,  //!< Часовой пояс UTC-8
    TIME_ZONE_UTC_MINUS7    = -700,  //!< Часовой пояс UTC-7
    TIME_ZONE_UTC_MINUS6    = -600,  //!< Часовой пояс UTC-6
    TIME_ZONE_UTC_MINUS5    = -500,  //!< Часовой пояс UTC-5
    TIME_ZONE_UTC_MINUS4    = -400,  //!< Часовой пояс UTC-4
    TIME_ZONE_UTC_MINUS3_30 = -330,  //!< Часовой пояс UTC-3:30
    TIME_ZONE_UTC_MINUS3    = -300,  //!< Часовой пояс UTC-3
    TIME_ZONE_UTC_MINUS2    = -200,  //!< Часовой пояс UTC-2
    TIME_ZONE_UTC_MINUS1    = -100,  //!< Часовой пояс UTC-1
    TIME_ZONE_UTC_PLUS0     = 0,     //!< Часовой пояс UTC+0
    TIME_ZONE_UTC_PLUS1     = 100,   //!< Часовой пояс UTC+1
    TIME_ZONE_UTC_PLUS2     = 200,   //!< Часовой пояс UTC+2
    TIME_ZONE_UTC_PLUS3     = 300,   //!< Часовой пояс UTC+3
    TIME_ZONE_UTC_PLUS3_30  = 330,   //!< Часовой пояс UTC+3:30
    TIME_ZONE_UTC_PLUS4     = 400,   //!< Часовой пояс UTC+4
    TIME_ZONE_UTC_PLUS4_30  = 430,   //!< Часовой пояс UTC+4:30
    TIME_ZONE_UTC_PLUS5     = 500,   //!< Часовой пояс UTC+5
    TIME_ZONE_UTC_PLUS5_30  = 530,   //!< Часовой пояс UTC+5:30
    TIME_ZONE_UTC_PLUS5_45  = 545,   //!< Часовой пояс UTC+5:45
    TIME_ZONE_UTC_PLUS6     = 600,   //!< Часовой пояс UTC+6
    TIME_ZONE_UTC_PLUS6_30  = 630,   //!< Часовой пояс UTC+6:30
    TIME_ZONE_UTC_PLUS7     = 700,   //!< Часовой пояс UTC+7
    TIME_ZONE_UTC_PLUS8     = 800,   //!< Часовой пояс UTC+8
    TIME_ZONE_UTC_PLUS8_45  = 845,   //!< Часовой пояс UTC+8:45
    TIME_ZONE_UTC_PLUS9     = 900,   //!< Часовой пояс UTC+9
    TIME_ZONE_UTC_PLUS9_30  = 930,   //!< Часовой пояс UTC+9:30
    TIME_ZONE_UTC_PLUS10    = 1000,  //!< Часовой пояс UTC+10
    TIME_ZONE_UTC_PLUS10_30 = 1030,  //!< Часовой пояс UTC+10:30
    TIME_ZONE_UTC_PLUS11    = 1100,  //!< Часовой пояс UTC+11
    TIME_ZONE_UTC_PLUS12    = 1200,  //!< Часовой пояс UTC+12
    TIME_ZONE_UTC_PLUS12_45 = 1245,  //!< Часовой пояс UTC+12:45
    TIME_ZONE_UTC_PLUS13    = 1300,  //!< Часовой пояс UTC+13
    TIME_ZONE_UTC_PLUS14    = 1400   //!< Часовой пояс UTC+14
  };


  /**
   * \brief Проверка правильности задания переменной часового пояса
   * \param time_zone Переменная часового пояса
   * \details
   * Переменная задается в формате ЧЧММ, где:
   * - ЧЧММ задается от -1200 до 1400
   * - ММ не может быть больше 59
   * \retval true  Переменная задана правильно
   * \retval false Переменная задана неправильно
   */
  bool IsTimeZoneValid(int16_t time_zone);

  /**
   * \}
   * \noop addtogroup timeMenu_base
   */
}