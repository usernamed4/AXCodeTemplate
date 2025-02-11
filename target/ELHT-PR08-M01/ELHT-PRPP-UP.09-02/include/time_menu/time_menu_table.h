/**
 * \file time_menu/time_menu_table.h
 * \brief Таблица параметров меню времени
 * \ingroup timeMenu_menu_table
 */

#pragma once

#include "menu.h"
#include "time_menu/base.h"

namespace time_menu
{
  /**
   * \addtogroup timeMenu_menu_table
   * \{
  */

  /**
   * \brief Параметр "Секунды"
   * \details
   * max_value = 60 для учета дополнительной секунды (leap second)
   */ 
  inline const menu::var<int16_t> seconds{{
    .min_value {0},
    .max_value {60}
  }};

  //! Параметр "Минуты"
  inline const menu::var<int16_t> minutes{{
    .min_value {0},
    .max_value {59}
  }};

  //! Параметр "Часы"
  inline const menu::var<int16_t> hours{{
    .min_value {0},
    .max_value {23}
  }};

  //! Параметр "День месяца"
  inline const menu::var<int16_t> date{{
    .default_value {1},
    .min_value {1},
    .max_value {31}
  }};

  //! Параметр "Месяц"
  inline const menu::var<int16_t> month{{
    .default_value {JANUARY},
    .min_value {JANUARY},
    .max_value {DECEMBER}
  }};

  //! Параметр "Год"
  inline const menu::var<int16_t> year{{
    .default_value {2023},
    .min_value     {1970},
    .max_value     {2100}
  }};

  //! Параметр "День недели"
  inline const menu::var<int16_t> day_of_week{{
    .flags {
      menu::flag::VISIBLE | menu::flag::RESET
    },
    .default_value {MONDAY},
    .min_value {MONDAY},
    .max_value {SUNDAY}
  }};

  /**
   * \brief Параметр "Часовой пояс" в формате ЧЧММ
   * \details
   * При задании значения ММ не может превышать 59
   */
  inline const menu::var<int16_t> time_zone{{
    .default_value {0},
    .min_value     {-1200},
    .max_value     {1400},
    .check_value_f {
      menu::var<int16_t>::check_value_function_t::create<IsTimeZoneValid>()
    }
  }};

  //! Информация о переменных меню времени
  inline const menu::TableInfo menu_info{
    seconds,
    minutes,
    hours,
    date,
    month,
    year,
    day_of_week,
    time_zone
  };
  //! Тип данных таблицы параметров времени
  using TimeTable = decltype(menu::Table{menu_info});

  /**
   * \}
   * \noop addtogroup timeMenu_menu_table
   */
}