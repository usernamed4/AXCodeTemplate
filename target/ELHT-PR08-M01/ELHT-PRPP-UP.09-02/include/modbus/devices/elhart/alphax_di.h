/**
 * \file modbus/devices/elhart/alphax_di.h
 * \brief Модули дискретного ввода Alpha-X DI, опрашиваемые по Modbus
 */

#pragma once

#include "modbus/devices/elhart/alphax_module.h"


namespace modbus::devices::elhart
{
  /**
   * \brief Alpha-X DI Модуль ввода дискретный
   * \details
   * Класс опрашивает и хранит только оперативные параметры
   */
  class AlphaX_DI : public AlphaXDevice
  {
  public:
    //! Номера групп дискретных входов
    enum DINPUT_GROUP
    {
      //! Группа дискретных входов модуля
      DINPUT_GROUP_DISCRETE        = 0,
      //! Группа входов наличия сигнала на дискретных входах модуля
      DINPUT_GROUP_DISCRETE_SIGNAL = 1,
      //! Кол-во групп дискретных входов
      DINPUT_GROUP_SIZE            = 2
    };

    //! Номера каналов дискретных входов
    enum DINPUT_CHANNEL
    {
      //! Начало каналов дискретных входов
      DINPUT_CHANNEL_START = 0,
      //! Дискретный вход №1
      DINPUT_CHANNEL1      = 0 + DINPUT_CHANNEL_START,
      //! Дискретный вход №2
      DINPUT_CHANNEL2      = 1 + DINPUT_CHANNEL_START,
      //! Дискретный вход №3
      DINPUT_CHANNEL3      = 2 + DINPUT_CHANNEL_START,
      //! Дискретный вход №4
      DINPUT_CHANNEL4      = 3 + DINPUT_CHANNEL_START,
      //! Дискретный вход №5
      DINPUT_CHANNEL5      = 4 + DINPUT_CHANNEL_START,
      //! Дискретный вход №6
      DINPUT_CHANNEL6      = 5 + DINPUT_CHANNEL_START,
      //! Дискретный вход №7
      DINPUT_CHANNEL7      = 6 + DINPUT_CHANNEL_START,
      //! Дискретный вход №8
      DINPUT_CHANNEL8      = 7 + DINPUT_CHANNEL_START,
      //! Дискретный вход №9
      DINPUT_CHANNEL9      = 8 + DINPUT_CHANNEL_START,
      //! Дискретный вход №10
      DINPUT_CHANNEL10     = 9 + DINPUT_CHANNEL_START,
      //! Дискретный вход №11
      DINPUT_CHANNEL11     = 10 + DINPUT_CHANNEL_START,
      //! Дискретный вход №12
      DINPUT_CHANNEL12     = 11 + DINPUT_CHANNEL_START,
      //! Дискретный вход №13
      DINPUT_CHANNEL13     = 12 + DINPUT_CHANNEL_START,
      //! Дискретный вход №14
      DINPUT_CHANNEL14     = 13 + DINPUT_CHANNEL_START,
      //! Дискретный вход №15
      DINPUT_CHANNEL15     = 14 + DINPUT_CHANNEL_START,
      //! Дискретный вход №16
      DINPUT_CHANNEL16     = 15 + DINPUT_CHANNEL_START,
      //! Кол-во каналов дискретных входов
      DINPUT_CHANNEL_SIZE  = 16,

      //! Начало каналов сигналов на дискретных входах
      DINPUT_SIGNAL_DINPUT_START     = DINPUT_GROUP_DISCRETE_SIGNAL * 32,
      //! Наличие сигнала на дискретном входе №1
      DINPUT_SIGNAL_DINPUT_CHANNEL1  = 0 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №2
      DINPUT_SIGNAL_DINPUT_CHANNEL2  = 1 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №3
      DINPUT_SIGNAL_DINPUT_CHANNEL3  = 2 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №4
      DINPUT_SIGNAL_DINPUT_CHANNEL4  = 3 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №5
      DINPUT_SIGNAL_DINPUT_CHANNEL5  = 4 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №6
      DINPUT_SIGNAL_DINPUT_CHANNEL6  = 5 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №7
      DINPUT_SIGNAL_DINPUT_CHANNEL7  = 6 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №8
      DINPUT_SIGNAL_DINPUT_CHANNEL8  = 7 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №9
      DINPUT_SIGNAL_DINPUT_CHANNEL9  = 8 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №10
      DINPUT_SIGNAL_DINPUT_CHANNEL10 = 9 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №11
      DINPUT_SIGNAL_DINPUT_CHANNEL11 = 10 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №12
      DINPUT_SIGNAL_DINPUT_CHANNEL12 = 11 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №13
      DINPUT_SIGNAL_DINPUT_CHANNEL13 = 12 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №14
      DINPUT_SIGNAL_DINPUT_CHANNEL14 = 13 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №15
      DINPUT_SIGNAL_DINPUT_CHANNEL15 = 14 + DINPUT_SIGNAL_DINPUT_START,
      //! Наличие сигнала на дискретном входе №16
      DINPUT_SIGNAL_DINPUT_CHANNEL16 = 15 + DINPUT_SIGNAL_DINPUT_START,
      //! Кол-во каналов наличия сигнала на дискретных входах
      DINPUT_SIGNAL_DINPUT_SIZE      = 16,
    };

    //! Таблица параметров "Настройки дискретных входов"
    using DInputsTable = alphax_parameter::DInputsTable;
    //! Таблица параметров "Настройки Modbus"
    using ModbusTable  = alphax_parameter::ModbusTable;
    //! Таблица параметров "Сохранение параметров"
    using SaveTable    = alphax_parameter::SaveTable;

    /**
     * \brief Описание таблицы оперативных параметров Alpha-X DI
     */
    inline static const menu::TableInfo operate_table_info{
      alphax_parameter::dinputs,
      alphax_parameter::doutputs,
      alphax_parameter::dinputs_signal
    };

    /**
     * \brief Таблица используемых оперативных параметров Alpha-X DI
     */
    class OperateTable : public decltype(menu::Table{operate_table_info})
    {
    public:
      using base_class = decltype(menu::Table{operate_table_info});
      using start_register = 
        etl::integral_constant<uint16_t, alphax_parameter::REGISTERS::DINPUTS>;

      OperateTable(elh::mutex* mutex=nullptr)
      : base_class{operate_table_info, mutex}
      {
      }
    };

    elh::mutex mutex;
    OperateTable operate_menu{&mutex};

    /**
     * \brief Конструктор модуля Alpha-X DI
     * \param address Адрес модуля в сети Modbus
     */
    AlphaX_DI(uint8_t address)
    : AlphaXDevice{{
        {
          {
            .poll_channels {_poll_channel_storage},
            .dinputs       {_dinputs_storage},
            .doutputs      {},
            .ainputs       {},
            .aoutputs      {}
          },
          .address {address}
        },
        .doutputs_signal_pos {0},
        .ainputs_status_pos  {0},
        .aoutputs_status_pos {0},
        .dinputs_size        {DINPUT_CHANNEL_SIZE},
        .doutputs_size       {0},
        .ainputs_size        {0},
        .aoutputs_size       {0},
        .operate_parameters  {operate_menu}
      }},
      _operate_poll_channel{{
        {
          .tree       {_operate_tree},
          .auto_write {false}
        },
        .address        {address},
        .function_write {0x10},
        .function_read  {0x03}
      }}
    {
    }

  private:
    const menu::TableNode<1> _operate_root {
      OperateTable::start_register::value, operate_menu
    };
    ModbusTree<1> _operate_tree {_operate_root};
    ModbusPollChannel _operate_poll_channel;

    const etl::array<device::PollChannel*, 1> _poll_channel_storage{
      &_operate_poll_channel
    };
    etl::array<device::DInputsGroup, DINPUT_GROUP_SIZE> _dinputs_storage{
      device::DInputsGroup{DINPUT_CHANNEL_SIZE, 0x00},
      device::DInputsGroup{DINPUT_CHANNEL_SIZE, 0x00}
    };

  public:
    // Переменные для обратной совместимости с предыдущими версиями библиотеки

    /**
     * \brief Структура списка групп дискретных входов прошлых версий
     * для обратной совместимости
     * \deprecated
     * \internal
     */
    struct DINPUTS
    {
      /**
       * \brief Перечисление списка групп дискретных входов прошлых версий
       * для обратной совместимости
       * \deprecated
       * \details
       * В новых проектах следует использовать \ref DINPUT_GROUP
       */
      enum NUMBER
      {
        DISCRETE        = DINPUT_GROUP_DISCRETE,
        DISCRETE_SIGNAL = DINPUT_GROUP_DISCRETE_SIGNAL,
        SIZE            = DINPUT_GROUP_SIZE
      };
    };
  };
}
