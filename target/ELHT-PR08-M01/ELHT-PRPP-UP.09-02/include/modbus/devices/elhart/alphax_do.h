/**
 * \file modbus/devices/elhart/alphax_do.h
 * \brief Модули дискретного вывода Alpha-X DO, опрашиваемые по Modbus
 */

#pragma once

#include "modbus/devices/elhart/alphax_module.h"


namespace modbus::devices::elhart
{
  /**
   * \brief Alpha-X DO Модуль вывода дискретный
   * \details
   * Класс опрашивает и хранит только оперативные параметры
   */
  class AlphaX_DO : public AlphaXDevice
  {
  public:
    //! Номера групп дискретных входов
    enum DINPUT_GROUP
    {
      //! Фактическое состояние дискретных выходов
      DINPUT_GROUP_DOUTPUTS_SIGNAL = 0,
      //! Кол-во групп дискретных входов
      DINPUT_GROUP_SIZE            = 1
    };

    //! Номера каналов дискретных входов
    enum DINPUT_CHANNEL
    {
      //! Начало каналов фактического состояния дискретных выходов
      DINPUT_SIGNAL_DOUTPUT_START     = DINPUT_GROUP_DOUTPUTS_SIGNAL * 32,
      //! Фактическое состояние дискретного выхода №1
      DINPUT_SIGNAL_DOUTPUT_CHANNEL1  = 0 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №2
      DINPUT_SIGNAL_DOUTPUT_CHANNEL2  = 1 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №3
      DINPUT_SIGNAL_DOUTPUT_CHANNEL3  = 2 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №4
      DINPUT_SIGNAL_DOUTPUT_CHANNEL4  = 3 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №5
      DINPUT_SIGNAL_DOUTPUT_CHANNEL5  = 4 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №6
      DINPUT_SIGNAL_DOUTPUT_CHANNEL6  = 5 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №7
      DINPUT_SIGNAL_DOUTPUT_CHANNEL7  = 6 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №8
      DINPUT_SIGNAL_DOUTPUT_CHANNEL8  = 7 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №9
      DINPUT_SIGNAL_DOUTPUT_CHANNEL9  = 8 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №10
      DINPUT_SIGNAL_DOUTPUT_CHANNEL10 = 9 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №11
      DINPUT_SIGNAL_DOUTPUT_CHANNEL11 = 10 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №12
      DINPUT_SIGNAL_DOUTPUT_CHANNEL12 = 11 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №13
      DINPUT_SIGNAL_DOUTPUT_CHANNEL13 = 12 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №14
      DINPUT_SIGNAL_DOUTPUT_CHANNEL14 = 13 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №15
      DINPUT_SIGNAL_DOUTPUT_CHANNEL15 = 14 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Фактическое состояние дискретного выхода №16
      DINPUT_SIGNAL_DOUTPUT_CHANNEL16 = 15 + DINPUT_SIGNAL_DOUTPUT_START,
      //! Кол-во каналов фактического состояния дискретных выходов
      DINPUT_SIGNAL_DOUTPUT_SIZE      = 16
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
      //! Дискретный выход №9
      DOUTPUT_CHANNEL9      = 8  + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №10
      DOUTPUT_CHANNEL10     = 9  + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №11
      DOUTPUT_CHANNEL11     = 10 + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №12
      DOUTPUT_CHANNEL12     = 11 + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №13
      DOUTPUT_CHANNEL13     = 12 + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №14
      DOUTPUT_CHANNEL14     = 13 + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №15
      DOUTPUT_CHANNEL15     = 14 + DOUTPUT_CHANNEL_START, 
      //! Дискретный выход №16
      DOUTPUT_CHANNEL16     = 15 + DOUTPUT_CHANNEL_START, 
      //! Кол-во каналов дискретных выходов 
      DOUTPUT_CHANNEL_SIZE  = 16
    };

    //! Таблица параметров "Широтно-импульсная модуляция (ШИМ)"
    using PWM_Table     = alphax_parameter::PWM_Table;
    //! Таблица параметров "Настройки дискретных выходов"
    using DOutputsTable = alphax_parameter::DOutputsTable;
    //! Таблица параметров "Настройки Modbus"
    using ModbusTable   = alphax_parameter::ModbusTable;
    //! Таблица параметров "Сохранение параметров"
    using SaveTable     = alphax_parameter::SaveTable;

    /**
     * \brief Описание таблицы оперативных параметров
     */
    inline static const menu::TableInfo operate_table_info{
      alphax_parameter::doutputs,
      alphax_parameter::dinputs_signal,
      alphax_parameter::doutputs_signal
    };

    /**
     * \brief Таблица используемых оперативных параметров Alpha-X DO
     */
    class OperateTable : public decltype(menu::Table{operate_table_info})
    {
    public:
      using base_class = decltype(menu::Table{operate_table_info});
      using start_register =
        etl::integral_constant<uint16_t, alphax_parameter::REGISTERS::DOUTPUTS>;

      OperateTable(elh::mutex* mutex=nullptr)
      : base_class{operate_table_info, mutex}
      {
      }
    };

    // Мьютекс доступа к параметрам модуля
    elh::mutex mutex{};
    //! Таблица оперативных параметров
    OperateTable operate_menu{&mutex};

    /**
     * \brief Конструктор модуля Alpha-X DO
     * \param address Адрес модуля в сети Modbus
     */
    AlphaX_DO(uint8_t address)
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
        .dinputs_size        {0},
        .doutputs_size       {DOUTPUT_CHANNEL_SIZE},
        .ainputs_size        {0},
        .aoutputs_size       {0},
        .operate_parameters  {operate_menu}
      }},
      _operate_poll_channel{{
        {
          .tree       {_operate_tree},
          .auto_write {true}
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
    ModbusTree<1>     _operate_tree{_operate_root};
    ModbusPollChannel _operate_poll_channel;

    const etl::array<device::PollChannel*, 1> _poll_channel_storage{
      &_operate_poll_channel
    };
    etl::array<device::DInputsGroup, DINPUT_GROUP_SIZE> _dinputs_storage{
      device::DInputsGroup{DINPUT_SIGNAL_DOUTPUT_SIZE}
    };
    etl::array<device::DOutputsGroup, DOUTPUT_GROUP_SIZE> _doutputs_storage{
      device::DOutputsGroup{DOUTPUT_CHANNEL_SIZE}
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
      enum NUMBER
      {
        DISCRETE = DOUTPUT_GROUP_DISCRETE,
        SIZE     = DOUTPUT_GROUP_SIZE
      };
    };

  };
}
