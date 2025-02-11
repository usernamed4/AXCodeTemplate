/**
 * \file modbus_menu.h
 * \brief Описание таблиц меню библиотеки Modbus
 */

#ifndef MODBUS_MENU_H
#define MODBUS_MENU_H

#include "modbus/modbus_base.h"
#include "device.h"

namespace modbus
{
  namespace parameter
  {
    //! Собственный адрес Slave-устройства
    inline const menu::var<uint8_t> device_address{{
      .flags         {
        menu::flag::VISIBLE | menu::flag::EDIT | menu::flag::RESET
      },
      .default_value {1},
      .min_value     {1},
      .max_value     {247}
    }};

    //! Адрес запроса к Slave-устройству от Master-а
    inline const menu::var<uint8_t> request_address{{
      .flags         {
        menu::flag::VISIBLE | menu::flag::EDIT | menu::flag::RESET
      },
      .default_value {1},
      .min_value     {0},
      .max_value     {255}
    }};

    //! Функция чтения
    inline const menu::var<uint8_t> function_read{{
      .default_value {0x03},
      .min_value     {0x01},
      .max_value     {0x04}
    }};

    /**
     * \brief Функция записи
     * \todo Реализовать список допустимых значений в меню
     */
    inline const menu::var<uint8_t> function_write{{
      .default_value {0x10},
      .min_value     {0x05},
      .max_value     {0x10}
    }};

    /**
     * \brief Максимальное количество регистров за один запрос
     * \details
     * Позволяет уменьшить количество регистров за один запрос.  
     * Параметр не может увеличить количество регистров больше допустимого
     * стандартом на функции Modbus
     */
    inline const menu::var<uint16_t> request_registers{{
      .default_value {125},
      .min_value     {1}
    }};

    //! Время ожидания ответа (мс)
    inline const menu::var<uint16_t> response_timeout{{
      .default_value {300}
    }};

    /**
     * \brief Пауза перед отправкой посылки в мс, не считая интервал тишины
     */
    inline const menu::var<uint16_t> poll_delay{{
      .default_value {0}
    }};

    /**
     * \brief Пауза после отправки широковещательного запроса или функции, не
     * требующей ответа
     * \details
     * - Время на обработку команды Master-а всеми Slave-устройствами
     */
    inline const menu::var<uint16_t> turnaround_delay{{
      .default_value {100}
    }};

    //! Кол-во попыток связи
    inline const menu::var<uint16_t> failed_attempts{{
      .default_value {3},
      .min_value     {1}
    }};

    //! Время возобновления опроса после неудачного соединения (мс)
    inline const menu::var<uint16_t> restore_timeout{{
      .default_value {3000}
    }};

    //! Первый адрес диапазона адресов конвертера Modbus
    inline const menu::var<uint16_t> converter_address_first{{
      .default_value {1},
      .min_value     {1},
      .max_value     {247}
    }};

    //! Последний адрес (включительно) диапазона адресов конвертера Modbus
    inline const menu::var<uint16_t> converter_address_last{{
      .default_value {247},
      .min_value     {1},
      .max_value     {247}
    }};

    //! Смещение диапазона адресов конвертера Modbus
    inline const menu::var<int16_t>  converter_address_shift{{
      .default_value {0},
      .min_value     {-246},
      .max_value     {246}
    }};
  }


  /**
   * \brief Описание таблицы параметров настроек Modbus Slave
   */
  inline const menu::TableInfo slave_menu_info{
    parameter::device_address
  };
  using SlaveTable = decltype(menu::Table{slave_menu_info});


  /**
   * \brief Описание таблицы параметров настроек Modbus Master
   */
  inline const menu::TableInfo master_menu_info{
    parameter::poll_delay,
    parameter::turnaround_delay,
    parameter::response_timeout,
    parameter::failed_attempts,
    parameter::restore_timeout
  };
  using MasterTable = decltype(menu::Table{master_menu_info});


  /**
   * \brief Описание таблицы параметров настроек канала опроса Modbus
  */
  inline const menu::TableInfo poll_channel_info{
    device::parameter::poll_period,
    device::parameter::auto_write,
    parameter::request_address,
    parameter::function_read,
    parameter::function_write,
    parameter::request_registers
  };

  /**
   * \brief Таблица параметров настроек канала опроса Modbus
  */
  class PollChannelTable : public decltype(menu::Table{poll_channel_info})
  {
    using base_table = decltype(menu::Table{poll_channel_info});
  public:
    PollChannelTable(elh::mutex* mutex=nullptr)
    : base_table{poll_channel_info, mutex}
    {
    }
  };


  /**
   * \brief Переменная регистра Modbus
   */
  inline const menu::var<uint16_t> modbus_register;

  /**
   * \brief Описание таблицы параметров из регитров Modbus
   */
  inline const menu::TableInfo modbus_register_menu_info{modbus_register};

  //! Тип описания таблицы параметров из регистров Modbus
  using ModbusRegisterMenuInfo = decltype(modbus_register_menu_info);

  /**
   * \brief Таблица параметров Modbus-регистров
   * \tparam SIZE Кол-во Modbus-регистров в таблице
   */
  template<size_t SIZE>
  using ModbusRegisterTable = menu::Table<SIZE, ModbusRegisterMenuInfo>;

  /**
   * \brief Переменная Coil для использования в функциях Coils и Discrete Inputs
   * \details
   * 
   */
  inline const menu::var<bool> modbus_coil{{
    .flags {
      menu::flag::VISIBLE | menu::flag::EDIT | menu::flag::RESET 
    },
    .default_value {false}
  }};


  /**
   * \brief Описание таблицы параметров для использования в функциях Coils и
   * Discrete Inputs
   */
  inline const menu::TableInfo modbus_coil_menu_info{modbus_coil};

  //! Тип описания таблицы параметров Coils
  using ModbusCoilMenuInfo = decltype(modbus_coil_menu_info);

  /**
   * \brief Тип таблицы параметров для Coils и Discrete Inputs
   * \tparam SIZE Кол-во Coils/Discrete Inputs в таблице
   */
  template<size_t SIZE>
  using ModbusCoilTable = menu::Table<SIZE, ModbusCoilMenuInfo>;

  /**
   * \brief Тип дерева регистров Modbus
   * \details
   * Один элемент дерева занимает 2 байта (размер регистра Modbus)
   */
  template<size_t SIZE>
  class ModbusTree : public menu::TableTree<SIZE>
  {
  public:
    using base_class = menu::TableTree<SIZE>;

    ModbusTree(const menu::iTableNode& root)
    : base_class{root, modbus::REGISTER_SIZE, {}}
    {
    }
  };
}

#endif /* MODBUS_MENU_H */
