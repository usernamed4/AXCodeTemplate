/**
 * \file time_menu/time_struct.h
 * \brief Функции работы со временем через структуру времени и даты
 * \ingroup timeMenu_struct
 */

#pragma once

#include "time_menu/base.h"

namespace time_menu
{
  /**
   * \addtogroup timeMenu_struct
   * \{
   */

  /**
   * \brief Структура времени и даты
   * \details
   * Размер переменных соответствует размеру регистра Modbus
   */
  struct TimeStruct
  {
    int16_t seconds;     //!< Секунды
    int16_t minutes;     //!< Минуты
    int16_t hours;       //!< Часы
    int16_t date;        //!< День месяца
    int16_t month;       //!< Месяц (см. перечисление \ref MONTHS)
    int16_t year;        //!< Год
    int16_t day_of_week; //!< День недели (см. перечисление \ref DAY_OF_WEEK)
    int16_t time_zone;   //!< Часовой пояс
  };


  /**
   * \brief Возвращает UNIX-время со смещением часового пояса
   * \param time UNIX-время без смещения
   * \param time_zone Часовой пояс в формате ЧЧММ
   * (см. \ref time_menu::TIME_ZONE)
   * \details
   * Для перевода времени с заданным часовым поясом в UTC+0 необходимо
   * записать в time_zone противоположный часовой пояс
   * (например, -300 для перевода из UTC+3 в UTC+0)
   * \return Время в секундах в указанном часовом поясе
   * \retval 0 Неправильно задан часовой пояс или время равно нулю секунд
   * с 01.01.1970г.
   */
  time_type GetTimeUTC(time_type time, int16_t time_zone);


  /**
   * \brief Возвращает структуру времени и даты
   * \param time Время в секундах, прошедшее с 01.01.1970г (UNIX-время)
   */
  TimeStruct GetTimeStruct(const time_type time, int16_t time_zone=0);

  /**
   * \brief Преобразует структуру времени и даты в unix-время в секундах
   * \param ts Структура времени и даты
   * \details
   * В преобразовании не участвует день недели и часовой пояс
   * 
   * \return Время в секундах, прошедшее с 01.01.1970г (UNIX-время)
   */
  time_type GetTimeFromStruct(const TimeStruct& ts);

  /**
   * \}
   * \noop addtogroup timeMenu_struct
   */
}