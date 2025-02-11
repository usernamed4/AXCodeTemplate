/**
 * \file kernel/kernel_api_serial_menu.h
 * \brief Меню настроек COM-порта
 */

#pragma once

#include "menu.h"

namespace kernel::serial
{
  namespace parameter
  {
    //! Скорость обмена
    inline const menu::var<uint32_t> baudrate{{
      .flags         {
        menu::flag::VISIBLE | menu::flag::EDIT | /*flag::SAVE |*/ menu::flag::RESET
      },
      .default_value {115200},
      .min_value     {2400},
      .max_value     {500000}
    }};

    //! Четность
    inline const menu::var<uint16_t> parity{{
      .flags         {
        menu::flag::VISIBLE | menu::flag::EDIT | /*flag::SAVE |*/ menu::flag::RESET
      },
      .default_value {0},
      .min_value     {0},
      .max_value     {2}
    }};

    //! Кол-во стоп-битов
    inline const menu::var<uint16_t> stop_bits{{
      .flags         {
        menu::flag::VISIBLE | menu::flag::EDIT | /*flag::SAVE |*/ menu::flag::RESET
      },
      .default_value {1},
      .min_value     {1},
      .max_value     {2}
    }};
  }

  //! Список переменных меню настроек последовательного порта
  inline const menu::TableInfo serial_menu_info{
    parameter::baudrate,
    parameter::parity,
    parameter::stop_bits
  };
  //! Тип меню настроек последовательного порта
  using SerialInterfaceMenu = decltype(menu::Table{serial_menu_info});
}
