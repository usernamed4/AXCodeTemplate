/**
 * \file kernel/kernel_api_modbus.h
 * \brief API задач Modbus
 */

#pragma once

#include "kernel/kernel_requests.h"
#include "kernel/kernel_api_serial_menu.h"
#include "modbus/master_requests.h"
#include "modbus/modbus_menu.h"

namespace kernel::tasks_modbus
{
  /**
   * \addtogroup kernel_api_modbus
   * \{
   */

  /**
   * \brief Описание таблицы параметров настроек Modbus Slave
   * для последовательного порта
   */
  inline const menu::TableInfo slave_settings_info{
    serial::parameter::baudrate,
    serial::parameter::parity,
    serial::parameter::stop_bits,
    modbus::parameter::device_address,
    modbus::parameter::poll_delay
  };


  /**
   * \brief Тип таблицы параметров настроек Modbus Slave для последовательного
   * порта
   * \details
   * При создании объекта необходимо указать в конструкторе описание таблицы
   * параметров
   * \code{.cpp}
   * kernel::tasks_modbus::SlaveSettingsMenu table{
   *   kernel::tasks_modbus::slave_settings_info, mutex
   * };
   * \endcode
   */
  using SlaveSettingsMenu = decltype(menu::Table{slave_settings_info});


  /**
   * \brief Описание таблицы параметров настроек Modbus Converter
   * для последовательного порта
   */
  inline const menu::TableInfo converter_settings_info{
    serial::parameter::baudrate,
    serial::parameter::parity,
    serial::parameter::stop_bits,
    modbus::parameter::device_address,
    modbus::parameter::poll_delay,
    modbus::parameter::converter_address_first,
    modbus::parameter::converter_address_last,
    modbus::parameter::converter_address_shift
  };


  /**
   * \brief Тип таблицы параметров настроек Modbus Converter
   * для последовательного порта
   * \details
   * При создании объекта необходимо указать в конструкторе описание таблицы
   * параметров
   * \code{.cpp}
   * kernel::tasks_modbus::ConverterSettingsMenu table{
   *   kernel::tasks_modbus::converter_settings_info, mutex
   * };
   * \endcode
   */
  using ConverterSettingsMenu = decltype(menu::Table{converter_settings_info});


  /**
   * \brief Описание таблицы параметров настроек Modbus Master
   * для последовательного порта
   */
  inline const menu::TableInfo master_settings_info{
    serial::parameter::baudrate,
    serial::parameter::parity,
    serial::parameter::stop_bits,
    modbus::parameter::poll_delay,
    modbus::parameter::turnaround_delay,
    modbus::parameter::response_timeout,
    modbus::parameter::failed_attempts,
    modbus::parameter::restore_timeout
  };


  /**
   * \brief Тип таблицы параметров настроек Modbus Master для последовательного
   * порта
   * \details
   * При создании объекта необходимо указать в конструкторе описание таблицы
   * параметров
   * \code{.cpp}
   * kernel::tasks_modbus::MasterSettingsMenu table{
   *   kernel::tasks_modbus::master_settings_info, mutex
   * };
   * \endcode
   */
  using MasterSettingsMenu = decltype(menu::Table{master_settings_info});


  /**
   * \brief Запрос ядра к Modbus Master
   * \details
   * 
   */
  struct Request
  {
    //! Заголовок запроса ядра с обратной связью через очередь
    request::Header<Request> header{};
    //! Запрос Modbus Master библиотеки modbus
    modbus::Request request{}; 
  };

  /**
   * \}
   * \noop kernel_api_modbus
   */
}
