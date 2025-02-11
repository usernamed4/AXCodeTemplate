/**
 * \file event_log.h
 * \brief Модуль ведения журнала событий
 * \ingroup plc_eventlog
 */

#pragma once

#include "stdint.h"
#include "stddef.h"


namespace plc
{
  /**
   * \addtogroup plc_eventlog
   * \{
   */

  /**
   * \brief Структура данных для хранения событий
   */
  struct event_t
  {
    uint16_t code; //!< Код события 
    int64_t time; //!< Время регистрации события
  }; 


  /**
   * \brief Прочитать событие из журнала
   * 
   * \param index Порядковый номер события в журнале. 0 - последнее событие
   * \return Событие
   */
  event_t EventlogRead(size_t index);


  /**
   * \brief Добавить событие в журнал
   * 
   * \param event_code Код события от 1 до 99
   */
  void EventlogWrite(uint16_t event_code);


  /**
   * \brief Получить количество событий в журнале
   * 
   * \return size_t Количество событий в журнале
   */
  size_t EventlogGetCount();


  /**
   * \brief Получить максимально возможное количество событий в журнале
   * 
   * \return size_t Максимально возможное количество событий в журнале
   */
  size_t EventlogGetSize();

  /**
   * \}
   * \noop addtogroup plc_eventlog
   */
}
