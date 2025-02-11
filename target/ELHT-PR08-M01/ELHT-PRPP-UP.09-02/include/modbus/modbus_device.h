/**
 * \file modbus/modbus_device.h
 * \brief Интерфейс устройства Modbus
 */

#pragma once

#include "device.h"
#include "modbus/modbus_base.h"
#include "modbus/modbus_menu.h"


namespace modbus
{
  /**
   * \brief Шаблон канала опроса Modbus-устройства
   * \tparam N кол-во таблиц параметров для опроса в канале
   * \details
   * Задает тип настроек канала опроса и выделяет место для хранения
   * списка указателей на опрашиваемые таблицы
   */
  class ModbusPollChannel : public device::PollChannel
  {
    using base_class = device::PollChannel;
    PollChannelTable _settings_storage;

  protected:
    struct Init : public base_class::Init
    {
      uint8_t address           {1};
      uint8_t function_write    {16};
      uint8_t function_read     {3};
      uint8_t request_registers {};
      elh::mutex* mutex         {nullptr};
    };

  public:
    ModbusPollChannel(const Init& init)
    : base_class{init, _settings_storage},
      _settings_storage{init.mutex}
    {
      __setInitSettings(init);
      _settings_storage.set(parameter::request_address, init.address);
      _settings_storage.set(parameter::function_write, init.function_write);
      _settings_storage.set(parameter::function_read,  init.function_read);
      _settings_storage.set(
        parameter::request_registers, init.request_registers
      );
    }
  };


  /**
   * \brief Modbus-устройство для опроса
   */
  class Device : public device::iDevice
  {
  public:
    struct Init : public device::iDevice::Init
    {
      uint8_t address {1}; //!< Адрес Modbus
    };

    Device(const Init& init)
    : device::iDevice{init}
    {
    }

    /**
     * \brief Задает Modbus-адрес всем каналам опроса устройства
     */
    menu::SET_STATUS setAddress(uint8_t new_address)
    {
      for(auto& i : poll_channels)
      {
        menu::SET_STATUS set = i->settings.set(
          parameter::request_address, new_address
        );
        switch(set)
        {
          case menu::SET_STATUS::WRONG:
          case menu::SET_STATUS::ERROR:
          {
            return set;
          }
          default:
          {
            ;
          }
        }
      }
      return menu::SET_STATUS::OK;
    }
  };
}
