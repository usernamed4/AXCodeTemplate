/**
 * \file kernel/kernel_api_rtc.h
 * \brief API часов реального времени (RTC) ядра
 * \ingroup kernel_api_rtc
 */

#pragma once

#include "time_menu/time_menu.h"
#include "etl/string.h"


namespace kernel::rtc
{
  /**
   * \addtogroup kernel_api_rtc
   * \{
   */

  /**
   * \brief Считывает системное UNIX-время из ядра
   * \return Время в секундах с 01.01.1970г
  */
  time_menu::time_type GetUnixTime();

  /**
   * \brief Возвращает копию таблицы параметров системных часов реального
   * времени
   */
  time_menu::TimeTable GetTimeMenu();

  /**
   * \brief Возвращает структуру часов реального времени
   * \return Структура времени и даты на текущее UNIX-время
   */

  const time_menu::TimeStruct& getTimeStruct();

  /**
   * \brief Устанавливает системное время ядра
   * \param new_time Новое устанавливаемое время
   * \details
   * \retval true если время установлено успешно
   * \retval false если произошла ошибка установки времени
  */
  bool SetUnixTime(time_menu::time_type new_time);

  /**
   * \brief Возвращает текущие время и дату в формате строки
   * \details 
   * Формат строки: ЧЧ:ММ:СС ДД.ММ.ГГГГ
   * \return Строка длиной 20 символов
   */
  etl::string<20> GetTimeString();


  /**
   * \brief Преобразует время и дату структуры времени в строку
   * \param ts Преобразуемая структура времени
   */
  etl::string<20> GetTimeString(const time_menu::TimeStruct& ts);

  /**
   * \}
   * \noop addtogroup kernel_api_rtc
   */
}