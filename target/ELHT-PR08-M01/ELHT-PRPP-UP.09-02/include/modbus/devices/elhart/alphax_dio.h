/**
 * \file modbus/devices/elhart/alphax_dio.h
 * \brief Модули дискретного ввода Alpha-X DIO, опрашиваемые по Modbus
 */

#pragma once

#include "modbus/devices/elhart/alphax_module.h"


namespace modbus::devices::elhart
{
  /**
   * \brief Alpha-X DIO Модуль ввода-вывода дискретный
   * \details
   * Класс опрашивает и хранит только оперативные параметры
   */
  class AlphaX_DIO : public AlphaXDevice
  {
  public:
    //! Номера групп дискретных входов
    enum DINPUT_GROUP
    {
      //! Группа дискретных входов модуля
      DINPUT_GROUP_DISCRETE        = 0,
      //! Группа входов наличия сигнала на дискретных входах модуля
      DINPUT_GROUP_DISCRETE_SIGNAL = 1,
      //! Группа входов фактического состояние дискретных выходов
      DINPUT_GROUP_DOUTPUTS_SIGNAL = 2,
      //! Кол-во групп дискретных входов
      DINPUT_GROUP_SIZE            = 3
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
      //! Кол-во каналов дискретных входов
      DINPUT_CHANNEL_SIZE  = 8,

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
      //! Кол-во каналов наличия сигнала на дискретных входах
      DINPUT_SIGNAL_DINPUT_SIZE      = 8,

      //! Начало каналов фактического состояния дискретных выходов
      DINPUT_SIGNAL_DOUTPUT_START     = DINPUT_GROUP_DOUTPUTS_SIGNAL * 32,
      //! Фактическое состояние дискретного выхода
      DINPUT_SIGNAL_DOUTPUT_CHANNEL1  = 0 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода
      DINPUT_SIGNAL_DOUTPUT_CHANNEL2  = 1 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода
      DINPUT_SIGNAL_DOUTPUT_CHANNEL3  = 2 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода
      DINPUT_SIGNAL_DOUTPUT_CHANNEL4  = 3 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода
      DINPUT_SIGNAL_DOUTPUT_CHANNEL5  = 4 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода
      DINPUT_SIGNAL_DOUTPUT_CHANNEL6  = 5 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода
      DINPUT_SIGNAL_DOUTPUT_CHANNEL7  = 6 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода
      DINPUT_SIGNAL_DOUTPUT_CHANNEL8  = 7 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Кол-во каналов фактического состояния дискретных выходв
      DINPUT_SIGNAL_DOUTPUT_SIZE      = 8
    };

    //! Номера групп дискретных выходов
    enum DOUTPUT_GROUP
    {
      DOUTPUT_GROUP_DISCRETE = 0, //!< Группа дискретных выходов модуля
      DOUTPUT_GROUP_SIZE     = 1  //!< Кол-во групп дискретных выходов
    };

    //! Номера каналов дискретных выходов
    enum DOUTPUT_CHANNEL
    {
      //! Начало каналов дискретных выходов
      DOUTPUT_CHANNEL_START = 0,
      //! Дискретный выход №1
      DOUTPUT_CHANNEL1      = 0  + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №2
      DOUTPUT_CHANNEL2      = 1  + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №3
      DOUTPUT_CHANNEL3      = 2  + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №4
      DOUTPUT_CHANNEL4      = 3  + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №5
      DOUTPUT_CHANNEL5      = 4  + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №6
      DOUTPUT_CHANNEL6      = 5  + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №7
      DOUTPUT_CHANNEL7      = 6  + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №8
      DOUTPUT_CHANNEL8      = 7  + DOUTPUT_CHANNEL_START,
      //! Кол-во каналов дискретных выходов 
      DOUTPUT_CHANNEL_SIZE  = 8
    };

    //! Таблица параметров "Широтно-импульсная модуляция (ШИМ)"
    using PWM_Table     = alphax_parameter::PWM_Table;
    //! Таблица параметров "Настройки дискретных входов"
    using DInputsTable = alphax_parameter::DInputsTable;
    //! Таблица параметров "Настройки дискретных выходов"
    using DOutputsTable = alphax_parameter::DOutputsTable;
    //! Таблица параметров "Настройки Modbus"
    using ModbusTable   = alphax_parameter::ModbusTable;
    //! Таблица параметров "Сохранение параметров"
    using SaveTable     = alphax_parameter::SaveTable;

    /**
     * \brief Описание таблицы оперативных параметров Alpha-X DIO
     */
    inline static const menu::TableInfo operate_table_info{
      alphax_parameter::dinputs,
      alphax_parameter::doutputs,
      alphax_parameter::dinputs_signal,
      alphax_parameter::doutputs_signal
    };

    /**
     * \brief Таблица используемых оперативных параметров Alpha-X DIO
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

    //! Мьютекс доступа к параметрам модуля
    elh::mutex mutex;
    //! Таблица оперативных параметров
    OperateTable operate_menu{&mutex};

    /**
     * \brief Конструктор устройства Alpha-X DIO
     * \param address Адрес устройства в сети Modbus
     */
    AlphaX_DIO(uint8_t address)
    : AlphaXDevice{{
        {
          {
            .poll_channels {_poll_channel_storage},
            .dinputs       {_dinputs_storage},
            .doutputs      {_doutputs_storage},
            .ainputs       {},
            .aoutputs      {}
          },
          .address {address}
        },
        .doutputs_signal_pos {DINPUT_GROUP_DOUTPUTS_SIGNAL},
        .ainputs_status_pos  {0},
        .aoutputs_status_pos {0},
        .dinputs_size        {DINPUT_CHANNEL_SIZE},
        .doutputs_size       {DOUTPUT_CHANNEL_SIZE},
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
      device::DInputsGroup{DINPUT_CHANNEL_SIZE,  0x00}, // DISCRETE
      device::DInputsGroup{DINPUT_CHANNEL_SIZE,  0x00}, // DISCRETE_SIGNAL
      device::DInputsGroup{DOUTPUT_CHANNEL_SIZE, 0x00}  // DOUTPUTS_SIGNAL
    };
    etl::array<device::DOutputsGroup, DOUTPUT_GROUP_SIZE> _doutputs_storage{
      device::DOutputsGroup{DOUTPUT_CHANNEL_SIZE, 0x00} // DISCRETE
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
      enum NUMBER : uint8_t
      {
        DISCRETE        = DINPUT_GROUP_DISCRETE,
        DISCRETE_SIGNAL = DINPUT_GROUP_DISCRETE_SIGNAL,
        DOUTPUTS_SIGNAL = DINPUT_GROUP_DOUTPUTS_SIGNAL,
        SIZE            = DINPUT_GROUP_SIZE
      };
    };

    /**
     * \brief Структура списка групп дискретных выходов прошлых версий
     * для обратной совместимости
     * \deprecated
     * \internal
     */
    struct DOUTPUTS
    {
      /**
       * \brief Перечисление списка групп дискретных выходов прошлых версий
       * для обратной совместимости
       * \deprecated
       * \details
       * В новых проектах следует использовать \ref DOUTPUT_GROUP
       */
      enum NUMBER : uint8_t
      {
        DISCRETE = DOUTPUT_GROUP_DISCRETE,
        SIZE     = DOUTPUT_GROUP_SIZE
      };
    };
  };
}
