/**
 * \file time_menu/time_menu.h
 * \brief Функции работы с таблицей параметров времени
 * \ingroup timeMenu_menu
 */

#ifndef TIME_MENU_H
#define TIME_MENU_H

#include "time_menu/time_struct.h"
#include "time_menu/time_menu_table.h"

namespace time_menu
{
  /**
   * \addtogroup timeMenu_menu
   * \{
   */

  /**
   * \brief Обновляет таблицу параметров времени
   * \param[in] time Время в секундах, прошедшее с 01.01.1970г (UNIX-время)
   * \param[out] time_menu Таблица параметров времени
   * \details
   * Преобразует время time в значения таблицы параметров
   * 
   * \todo Добавить часовой пояс
   * 
   * \retval true  если меню обновлено
   * \retval false если при обновлении произошла ошибка
   */
  bool UpdateTimeMenu(const time_type time, menu::iTable& time_menu);


  /**
   * \brief Преобразует таблицу параметров времени в unix-время в секундах
   * \param[in] time_menu Таблица параметров времени
   * 
   * \todo Добавить часовой пояс
   * 
   * \return Время в секундах, прошедшее с 01.01.1970г (UNIX-время) в текущем
   * часовом поясе
   */
  time_type GetTimeFromMenu(menu::iTable& time_menu);

  /**
   * \}
   * \noop addtogroup timeMenu_menu
   */
}

#endif /* TIME_DATE_MENU_H */
