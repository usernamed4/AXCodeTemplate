/**
 * \file modbus/devices/elhart/alphax_module.h
 * \brief Модули ввода/вывода Alpha-X, опрашиваемые по Modbus
 */

#pragma once

#include "modbus/devices/elhart/elhart_device.h"
#include "modbus/devices/elhart/alphax_module_table.h"
#include "modbus/devices/elhart/alphax_module_modifications.h"

namespace modbus::devices::elhart
{
  /**
   * \brief Устройство семейства модулей Alpha-X
   * \details
   * Может использоваться как указатель на устройства Alpha-X.
   * Для самих устройств используются конкретные классы, наследуемые от данного
   */
  class AlphaXDevice : public ElhartDevice
  {
  protected:
    struct Init : public Device::Init
    {
      uint8_t doutputs_signal_pos; //!< Группа статуса дискретных выходов
      uint8_t ainputs_status_pos;  //!< Группа статуса аналоговых входов
      uint8_t aoutputs_status_pos; //!< Группа статуса аналоговых выходов
      uint8_t dinputs_size;        //!< Кол-во дискретных входов в модуле
      uint8_t doutputs_size;       //!< Кол-во дискретных выходов в модуле
      uint8_t ainputs_size;        //!< Кол-во аналоговых входов в модуле
      uint8_t aoutputs_size;       //!< Кол-во аналоговых выходов в модуле
      menu::iTable& operate_parameters;
    };

    const uint8_t _doutputs_signal_group; //!< Группа статуса дискретных выходов
    const uint8_t _ainputs_status_group;  //!< Группа статуса аналоговых входов
    const uint8_t _aoutputs_status_group; //!< Группа статуса аналоговых выходов
    const uint8_t _dinputs_size_max;  //!< Макс. дискретных входов в классе
    const uint8_t _doutputs_size_max; //!< Макс. дискретных выходов в классе
    const uint8_t _ainputs_size_max;  //!< Макс. аналоговых входов в классе
    const uint8_t _aoutputs_size_max; //!< Макс. аналоговых выходов в классе
    uint8_t _dinputs_size;  //!< Кол-во доступных дискретных входов в модуле
    uint8_t _doutputs_size; //!< Кол-во доступных дискретных выходов в модуле
    uint8_t _ainputs_size;  //!< Кол-во доступных аналоговых входов в модуле
    uint8_t _aoutputs_size; //!< Кол-во доступных аналоговых выходов в модуле

    AlphaXDevice(const Init& init)
    : ElhartDevice           {init},
      _doutputs_signal_group {init.doutputs_signal_pos},
      _ainputs_status_group  {init.ainputs_status_pos},
      _aoutputs_status_group {init.aoutputs_status_pos},
      _dinputs_size_max      {init.dinputs_size},
      _doutputs_size_max     {init.doutputs_size},
      _ainputs_size_max      {init.ainputs_size},
      _aoutputs_size_max     {init.aoutputs_size},
      _dinputs_size          {init.dinputs_size},
      _doutputs_size         {init.doutputs_size},
      _ainputs_size          {init.ainputs_size},
      _aoutputs_size         {init.aoutputs_size},
      operate_parameters     {init.operate_parameters}
    {
    }

  public:
    //! Таблица оперативных параметров
    menu::iTable& operate_parameters;

    //! Возвращает кол-во доступных  дискретных входов в модуле
    uint8_t dinputsSize() const  { return _dinputs_size;  }
    //! Возвращает кол-во доступных дискретных выходов в модуле
    uint8_t doutputsSize() const { return _doutputs_size; }
    //! Возвращает кол-во доступных аналоговых входов в модуле
    uint8_t ainputsSize() const  { return _ainputs_size;  }
    //! Возвращает кол-во доступных аналоговых выходов в модуле
    uint8_t aoutputsSize() const { return _aoutputs_size; }

    void update() override;
  };


  /**
   * \brief Alpha-X Модуль ввода-вывода универсальный
   * \details
   * Класс опрашивает и хранит только оперативные параметры.
   * 
   * Позволяет работать с любым модулем Alpha-X, но для настройки нужно
   * использовать только функции конкретной модификации модуля.
   */
  class AlphaXModule : public AlphaXDevice
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
      //! Группа входов статуса работы аналогового входа №1
      DINPUT_GROUP_AINPUT1_STATUS  = 3,
      //! Группа входов статуса работы аналогового входа №2
      DINPUT_GROUP_AINPUT2_STATUS  = 4,
      //! Группа входов статуса работы аналогового входа №3
      DINPUT_GROUP_AINPUT3_STATUS  = 5,
      //! Группа входов статуса работы аналогового входа №4
      DINPUT_GROUP_AINPUT4_STATUS  = 6,
      //! Группа входов статуса работы аналогового входа №5
      DINPUT_GROUP_AINPUT5_STATUS  = 7,
      //! Группа входов статуса работы аналогового входа №6
      DINPUT_GROUP_AINPUT6_STATUS  = 8,
      //! Группа входов статуса работы аналогового входа №7
      DINPUT_GROUP_AINPUT7_STATUS  = 9,
      //! Группа входов статуса работы аналогового входа №8
      DINPUT_GROUP_AINPUT8_STATUS  = 10,
      //! Группа входов статуса работы аналогового выхода №1
      DINPUT_GROUP_AOUTPUT1_STATUS = 11,
      //! Группа входов статуса работы аналогового выхода №2
      DINPUT_GROUP_AOUTPUT2_STATUS = 12,
      //! Группа входов статуса работы аналогового выхода №3
      DINPUT_GROUP_AOUTPUT3_STATUS = 13,
      //! Группа входов статуса работы аналогового выхода №4
      DINPUT_GROUP_AOUTPUT4_STATUS = 14,
      //! Кол-во групп дискретных входов
      DINPUT_GROUP_SIZE            = 15
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
      DINPUT_SIGNAL_DOUTPUT_SIZE      = 16,

      //! Начало каналов статуса аналогового входа №1
      DINPUT_AINPUT1_STATUS_START            = DINPUT_GROUP_AINPUT1_STATUS * 32,
      //! Измерение аналогового входа №1 в норме, ошибки отсутствуют
      DINPUT_AINPUT1_STATUS_NO_ERRORS        = 0 + DINPUT_AINPUT1_STATUS_START,
      //! Измеренное значение аналогового входа №1 выше верхнего аварийного
      DINPUT_AINPUT1_STATUS_ALARM_HIGH       = 1 + DINPUT_AINPUT1_STATUS_START,
      //! Измеренное значение аналогового входа №1 ниже нижнего аварийного
      DINPUT_AINPUT1_STATUS_ALARM_LOW        = 2 + DINPUT_AINPUT1_STATUS_START,
      //! Обрыв датчика аналогового входа №1 (только для датчиков 4..20 мА)
      DINPUT_AINPUT1_STATUS_SENSOR_BREAK     = 3 + DINPUT_AINPUT1_STATUS_START,
      //! Нет связи с АЦП аналогового входа №1
      DINPUT_AINPUT1_STATUS_ADC_ERROR        = 4 + DINPUT_AINPUT1_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №1
      DINPUT_AINPUT1_STATUS_JUNCTION_ERROR   = 5 + DINPUT_AINPUT1_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №1
      DINPUT_AINPUT1_STATUS_ADJUSTMENT_ERROR = 6 + DINPUT_AINPUT1_STATUS_START,
      //! Кол-во каналов статуса аналогового входа №1
      DINPUT_AINPUT1_STATUS_SIZE             = 16,

      //! Начало каналов статуса аналогового входа №2
      DINPUT_AINPUT2_STATUS_START            = DINPUT_GROUP_AINPUT2_STATUS * 32,
      //! Измерение аналогового входа №2 в норме, ошибки отсутствуют
      DINPUT_AINPUT2_STATUS_NO_ERRORS        = 0 + DINPUT_AINPUT2_STATUS_START,
      //! Измеренное значение аналогового входа №2 выше верхнего аварийного
      DINPUT_AINPUT2_STATUS_ALARM_HIGH       = 1 + DINPUT_AINPUT2_STATUS_START,
      //! Измеренное значение аналогового входа №2 ниже нижнего аварийного
      DINPUT_AINPUT2_STATUS_ALARM_LOW        = 2 + DINPUT_AINPUT2_STATUS_START,
      //! Обрыв датчика аналогового входа №2 (только для датчиков 4..20 мА)
      DINPUT_AINPUT2_STATUS_SENSOR_BREAK     = 3 + DINPUT_AINPUT2_STATUS_START,
      //! Нет связи с АЦП аналогового входа №2
      DINPUT_AINPUT2_STATUS_ADC_ERROR        = 4 + DINPUT_AINPUT2_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №2
      DINPUT_AINPUT2_STATUS_JUNCTION_ERROR   = 5 + DINPUT_AINPUT2_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №2
      DINPUT_AINPUT2_STATUS_ADJUSTMENT_ERROR = 6 + DINPUT_AINPUT2_STATUS_START,
      //! Кол-во каналов статуса аналогового входа №2
      DINPUT_AINPUT2_STATUS_SIZE             = 16,

      //! Начало каналов статуса аналогового входа №3
      DINPUT_AINPUT3_STATUS_START            = DINPUT_GROUP_AINPUT3_STATUS * 32,
      //! Измерение аналогового входа №3 в норме, ошибки отсутствуют
      DINPUT_AINPUT3_STATUS_NO_ERRORS        = 0 + DINPUT_AINPUT3_STATUS_START,
      //! Измеренное значение аналогового входа №3 выше верхнего аварийного
      DINPUT_AINPUT3_STATUS_ALARM_HIGH       = 1 + DINPUT_AINPUT3_STATUS_START,
      //! Измеренное значение аналогового входа №3 ниже нижнего аварийного
      DINPUT_AINPUT3_STATUS_ALARM_LOW        = 2 + DINPUT_AINPUT3_STATUS_START,
      //! Обрыв датчика аналогового входа №3 (только для датчиков 4..20 мА)
      DINPUT_AINPUT3_STATUS_SENSOR_BREAK     = 3 + DINPUT_AINPUT3_STATUS_START,
      //! Нет связи с АЦП аналогового входа №3
      DINPUT_AINPUT3_STATUS_ADC_ERROR        = 4 + DINPUT_AINPUT3_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №3
      DINPUT_AINPUT3_STATUS_JUNCTION_ERROR   = 5 + DINPUT_AINPUT3_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №3
      DINPUT_AINPUT3_STATUS_ADJUSTMENT_ERROR = 6 + DINPUT_AINPUT3_STATUS_START,
      //! Кол-во каналов статуса аналогового входа №3
      DINPUT_AINPUT3_STATUS_SIZE             = 16,

      //! Начало каналов статуса аналогового входа №4
      DINPUT_AINPUT4_STATUS_START            = DINPUT_GROUP_AINPUT4_STATUS * 32,
      //! Измерение аналогового входа №4 в норме, ошибки отсутствуют
      DINPUT_AINPUT4_STATUS_NO_ERRORS        = 0 + DINPUT_AINPUT4_STATUS_START,
      //! Измеренное значение аналогового входа №4 выше верхнего аварийного
      DINPUT_AINPUT4_STATUS_ALARM_HIGH       = 1 + DINPUT_AINPUT4_STATUS_START,
      //! Измеренное значение аналогового входа №4 ниже нижнего аварийного
      DINPUT_AINPUT4_STATUS_ALARM_LOW        = 2 + DINPUT_AINPUT4_STATUS_START,
      //! Обрыв датчика аналогового входа №4 (только для датчиков 4..20 мА)
      DINPUT_AINPUT4_STATUS_SENSOR_BREAK     = 3 + DINPUT_AINPUT4_STATUS_START,
      //! Нет связи с АЦП аналогового входа №4
      DINPUT_AINPUT4_STATUS_ADC_ERROR        = 4 + DINPUT_AINPUT4_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №4
      DINPUT_AINPUT4_STATUS_JUNCTION_ERROR   = 5 + DINPUT_AINPUT4_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №4
      DINPUT_AINPUT4_STATUS_ADJUSTMENT_ERROR = 6 + DINPUT_AINPUT4_STATUS_START,
      //! Кол-во каналов статуса аналогового входа №4
      DINPUT_AINPUT4_STATUS_SIZE             = 16,

      //! Начало каналов статуса аналогового входа №5
      DINPUT_AINPUT5_STATUS_START            = DINPUT_GROUP_AINPUT5_STATUS * 32,
      //! Измерение аналогового входа №5 в норме, ошибки отсутствуют
      DINPUT_AINPUT5_STATUS_NO_ERRORS        = 0 + DINPUT_AINPUT5_STATUS_START,
      //! Измеренное значение аналогового входа №5 выше верхнего аварийного
      DINPUT_AINPUT5_STATUS_ALARM_HIGH       = 1 + DINPUT_AINPUT5_STATUS_START,
      //! Измеренное значение аналогового входа №5 ниже нижнего аварийного
      DINPUT_AINPUT5_STATUS_ALARM_LOW        = 2 + DINPUT_AINPUT5_STATUS_START,
      //! Обрыв датчика аналогового входа №5 (только для датчиков 4..20 мА)
      DINPUT_AINPUT5_STATUS_SENSOR_BREAK     = 3 + DINPUT_AINPUT5_STATUS_START,
      //! Нет связи с АЦП аналогового входа №5
      DINPUT_AINPUT5_STATUS_ADC_ERROR        = 4 + DINPUT_AINPUT5_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №5
      DINPUT_AINPUT5_STATUS_JUNCTION_ERROR   = 5 + DINPUT_AINPUT5_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №5
      DINPUT_AINPUT5_STATUS_ADJUSTMENT_ERROR = 6 + DINPUT_AINPUT5_STATUS_START,
      //! Кол-во каналов статуса аналогового входа №5
      DINPUT_AINPUT5_STATUS_SIZE             = 16,

      //! Начало каналов статуса аналогового входа №6
      DINPUT_AINPUT6_STATUS_START            = DINPUT_GROUP_AINPUT6_STATUS * 32,
      //! Измерение аналогового входа №6 в норме, ошибки отсутствуют
      DINPUT_AINPUT6_STATUS_NO_ERRORS        = 0 + DINPUT_AINPUT6_STATUS_START,
      //! Измеренное значение аналогового входа №6 выше верхнего аварийного
      DINPUT_AINPUT6_STATUS_ALARM_HIGH       = 1 + DINPUT_AINPUT6_STATUS_START,
      //! Измеренное значение аналогового входа №6 ниже нижнего аварийного
      DINPUT_AINPUT6_STATUS_ALARM_LOW        = 2 + DINPUT_AINPUT6_STATUS_START,
      //! Обрыв датчика аналогового входа №6 (только для датчиков 4..20 мА)
      DINPUT_AINPUT6_STATUS_SENSOR_BREAK     = 3 + DINPUT_AINPUT6_STATUS_START,
      //! Нет связи с АЦП аналогового входа №6
      DINPUT_AINPUT6_STATUS_ADC_ERROR        = 4 + DINPUT_AINPUT6_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №6
      DINPUT_AINPUT6_STATUS_JUNCTION_ERROR   = 5 + DINPUT_AINPUT6_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №6
      DINPUT_AINPUT6_STATUS_ADJUSTMENT_ERROR = 6 + DINPUT_AINPUT6_STATUS_START,
      //! Кол-во каналов статуса аналогового входа №6
      DINPUT_AINPUT6_STATUS_SIZE             = 16,

      //! Начало каналов статуса аналогового входа №7
      DINPUT_AINPUT7_STATUS_START            = DINPUT_GROUP_AINPUT7_STATUS * 32,
      //! Измерение аналогового входа №7 в норме, ошибки отсутствуют
      DINPUT_AINPUT7_STATUS_NO_ERRORS        = 0 + DINPUT_AINPUT7_STATUS_START,
      //! Измеренное значение аналогового входа №7 выше верхнего аварийного
      DINPUT_AINPUT7_STATUS_ALARM_HIGH       = 1 + DINPUT_AINPUT7_STATUS_START,
      //! Измеренное значение аналогового входа №7 ниже нижнего аварийного
      DINPUT_AINPUT7_STATUS_ALARM_LOW        = 2 + DINPUT_AINPUT7_STATUS_START,
      //! Обрыв датчика аналогового входа №7 (только для датчиков 4..20 мА)
      DINPUT_AINPUT7_STATUS_SENSOR_BREAK     = 3 + DINPUT_AINPUT7_STATUS_START,
      //! Нет связи с АЦП аналогового входа №7
      DINPUT_AINPUT7_STATUS_ADC_ERROR        = 4 + DINPUT_AINPUT7_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №7
      DINPUT_AINPUT7_STATUS_JUNCTION_ERROR   = 5 + DINPUT_AINPUT7_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №7
      DINPUT_AINPUT7_STATUS_ADJUSTMENT_ERROR = 6 + DINPUT_AINPUT7_STATUS_START,
      //! Кол-во каналов статуса аналогового входа №7
      DINPUT_AINPUT7_STATUS_SIZE             = 16,

      //! Начало каналов статуса аналогового входа №8
      DINPUT_AINPUT8_STATUS_START            = DINPUT_GROUP_AINPUT8_STATUS * 32,
      //! Измерение аналогового входа №8 в норме, ошибки отсутствуют
      DINPUT_AINPUT8_STATUS_NO_ERRORS        = 0 + DINPUT_AINPUT8_STATUS_START,
      //! Измеренное значение аналогового входа №8 выше верхнего аварийного
      DINPUT_AINPUT8_STATUS_ALARM_HIGH       = 1 + DINPUT_AINPUT8_STATUS_START,
      //! Измеренное значение аналогового входа №8 ниже нижнего аварийного
      DINPUT_AINPUT8_STATUS_ALARM_LOW        = 2 + DINPUT_AINPUT8_STATUS_START,
      //! Обрыв датчика аналогового входа №8 (только для датчиков 4..20 мА)
      DINPUT_AINPUT8_STATUS_SENSOR_BREAK     = 3 + DINPUT_AINPUT8_STATUS_START,
      //! Нет связи с АЦП аналогового входа №8
      DINPUT_AINPUT8_STATUS_ADC_ERROR        = 4 + DINPUT_AINPUT8_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №8
      DINPUT_AINPUT8_STATUS_JUNCTION_ERROR   = 5 + DINPUT_AINPUT8_STATUS_START,
      //! Ошибка компенсации температуры холодного спая аналогового входа №8
      DINPUT_AINPUT8_STATUS_ADJUSTMENT_ERROR = 6 + DINPUT_AINPUT8_STATUS_START,
      //! Кол-во каналов статуса аналогового входа №8
      DINPUT_AINPUT8_STATUS_SIZE             = 16,

      //! Начало каналов статуса аналогового выхода
      DINPUT_AOUTPUT1_STATUS_START          = DINPUT_GROUP_AOUTPUT1_STATUS * 32,
      //! Сигнал аналогового выхода №1 в норме (выход активирован)
      DINPUT_AOUTPUT1_STATUS_ENABLED        = 0 + DINPUT_AOUTPUT1_STATUS_START,
      //! Аварийное состояние аналогового выхода №1
      DINPUT_AOUTPUT1_STATUS_SAFE_STATE     = 1 + DINPUT_AOUTPUT1_STATUS_START,
      //! Отсутствует юстировка аналогового выхода №1
      DINPUT_AOUTPUT1_STATUS_ADJUSTMENT_ERR = 2 + DINPUT_AOUTPUT1_STATUS_START,
      //! Кол-во каналов статуса аналогового выхода №1
      DINPUT_AOUTPUT1_STATUS_SIZE           = 3,

      //! Начало каналов статуса аналогового выхода
      DINPUT_AOUTPUT2_STATUS_START          = DINPUT_GROUP_AOUTPUT2_STATUS * 32,
      //! Сигнал аналогового выхода №2 в норме (выход активирован)
      DINPUT_AOUTPUT2_STATUS_ENABLED        = 0 + DINPUT_AOUTPUT2_STATUS_START,
      //! Аварийное состояние аналогового выхода №2
      DINPUT_AOUTPUT2_STATUS_SAFE_STATE     = 1 + DINPUT_AOUTPUT2_STATUS_START,
      //! Отсутствует юстировка аналогового выхода №2
      DINPUT_AOUTPUT2_STATUS_ADJUSTMENT_ERR = 2 + DINPUT_AOUTPUT2_STATUS_START,
      //! Кол-во каналов статуса аналогового выхода №2
      DINPUT_AOUTPUT2_STATUS_SIZE           = 3,

      //! Начало каналов статуса аналогового выхода
      DINPUT_AOUTPUT3_STATUS_START          = DINPUT_GROUP_AOUTPUT3_STATUS * 32,
      //! Сигнал аналогового выхода №3 в норме (выход активирован)
      DINPUT_AOUTPUT3_STATUS_ENABLED        = 0 + DINPUT_AOUTPUT3_STATUS_START,
      //! Аварийное состояние аналогового выхода №3
      DINPUT_AOUTPUT3_STATUS_SAFE_STATE     = 1 + DINPUT_AOUTPUT3_STATUS_START,
      //! Отсутствует юстировка аналогового выхода №3
      DINPUT_AOUTPUT3_STATUS_ADJUSTMENT_ERR = 2 + DINPUT_AOUTPUT3_STATUS_START,
      //! Кол-во каналов статуса аналогового выхода №3
      DINPUT_AOUTPUT3_STATUS_SIZE           = 3,

      //! Начало каналов статуса аналогового выхода
      DINPUT_AOUTPUT4_STATUS_START          = DINPUT_GROUP_AOUTPUT4_STATUS * 32,
      //! Сигнал аналогового выхода №4 в норме (выход активирован)
      DINPUT_AOUTPUT4_STATUS_ENABLED        = 0 + DINPUT_AOUTPUT4_STATUS_START,
      //! Аварийное состояние аналогового выхода №4
      DINPUT_AOUTPUT4_STATUS_SAFE_STATE     = 1 + DINPUT_AOUTPUT4_STATUS_START,
      //! Отсутствует юстировка аналогового выхода №4
      DINPUT_AOUTPUT4_STATUS_ADJUSTMENT_ERR = 2 + DINPUT_AOUTPUT4_STATUS_START,
      //! Кол-во каналов статуса аналогового выхода №4
      DINPUT_AOUTPUT4_STATUS_SIZE           = 3
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

    //! Номера каналов аналоговых входов
    enum AINPUT_CHANNEL
    {
      //! Начало каналов аналоговых входов
      AINPUT_CHANNEL_START = 0,
      //! Измерительный вход №1
      AINPUT_CHANNEL1      = 0 + AINPUT_CHANNEL_START,
      //! Измерительный вход №2
      AINPUT_CHANNEL2      = 1 + AINPUT_CHANNEL_START,
      //! Измерительный вход №3
      AINPUT_CHANNEL3      = 2 + AINPUT_CHANNEL_START,
      //! Измерительный вход №4
      AINPUT_CHANNEL4      = 3 + AINPUT_CHANNEL_START,
      //! Измерительный вход №5
      AINPUT_CHANNEL5      = 4 + AINPUT_CHANNEL_START,
      //! Измерительный вход №6
      AINPUT_CHANNEL6      = 5 + AINPUT_CHANNEL_START,
      //! Измерительный вход №7
      AINPUT_CHANNEL7      = 6 + AINPUT_CHANNEL_START,
      //! Измерительный вход №8
      AINPUT_CHANNEL8      = 7 + AINPUT_CHANNEL_START,
      //! Кол-во аналоговых входов
      AINPUT_CHANNEL_SIZE  = 8
    };

    //! Номера каналов аналоговых выходов
    enum AOUTPUT_CHANNEL
    {
      //! Начало каналов аналоговых входов
      AOUTPUT_CHANNEL_START = 0,
      //! Аналоговый выход №1
      AOUTPUT_CHANNEL1      = 0 + AOUTPUT_CHANNEL_START, 
      //! Аналоговый выход №2
      AOUTPUT_CHANNEL2      = 1 + AOUTPUT_CHANNEL_START, 
      //! Аналоговый выход №3
      AOUTPUT_CHANNEL3      = 2 + AOUTPUT_CHANNEL_START, 
      //! Аналоговый выход №4
      AOUTPUT_CHANNEL4      = 3 + AOUTPUT_CHANNEL_START, 
      //! Кол-во аналоговых выходов
      AOUTPUT_CHANNEL_SIZE  = 4 
    };

    //! Таблица оперативных параметров
    using OperateTable  = alphax_parameter::OperateTable;
    //! Таблица параметров "Счетчики и частотомеры"
    using CountersTable = alphax_parameter::CountersTable;
    //! Таблица параметров "Широтно-импульсная модуляция (ШИМ)"
    using PWM_Table     = alphax_parameter::PWM_Table;
    //! Таблица параметров "Настройки дискретных входов"
    using DInputsTable  = alphax_parameter::DInputsTable;
    //! Таблица параметров "Настройки дискретных выходов"
    using DOutputsTable = alphax_parameter::DOutputsTable;
    //! Таблица параметров "Настройки аналоговых входов"
    using AInputsTable  = alphax_parameter::AInputsTable;
    //! Таблица параметров "Настройки аналоговых выходов"
    using AOutputsTable = alphax_parameter::AOutputsTable;
    //! Таблица параметров "Настройки Modbus"
    using ModbusTable   = alphax_parameter::ModbusTable;
    //! Таблица параметров "Сохранение параметров"
    using SaveTable     = alphax_parameter::SaveTable;

    //! Мьютекс доступа к параметрам модуля
    elh::mutex mutex{};
    //! Таблица оперативных параметров
    OperateTable operate_menu{&mutex};

    /**
     * \brief Конструктор универсального модуля Alpha-X
     * \param address Адрес модуля в сети Modbus
     */
    AlphaXModule(uint8_t address)
    : AlphaXDevice{{
        {
          {
            .poll_channels {_poll_channel_storage},
            .dinputs       {_dinputs_storage},
            .doutputs      {_doutputs_storage},
            .ainputs       {_ainputs_storage},
            .aoutputs      {_aoutputs_storage}
          },
          .address {address}
        },
        .doutputs_signal_pos {DINPUT_GROUP_DOUTPUTS_SIGNAL},
        .ainputs_status_pos  {DINPUT_GROUP_AINPUT1_STATUS},
        .aoutputs_status_pos {DINPUT_GROUP_AOUTPUT1_STATUS},
        .dinputs_size        {DINPUT_CHANNEL_SIZE},
        .doutputs_size       {DOUTPUT_CHANNEL_SIZE},
        .ainputs_size        {AINPUT_CHANNEL_SIZE},
        .aoutputs_size       {AOUTPUT_CHANNEL_SIZE},
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
    etl::array<device::DInputsGroup, DINPUT_GROUP_SIZE>  _dinputs_storage{
      device::DInputsGroup{DINPUT_CHANNEL_SIZE},
      device::DInputsGroup{DINPUT_CHANNEL_SIZE},
      device::DInputsGroup{DOUTPUT_CHANNEL_SIZE},
      device::DInputsGroup{DINPUT_AINPUT1_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AINPUT2_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AINPUT3_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AINPUT4_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AINPUT5_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AINPUT6_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AINPUT7_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AINPUT8_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AOUTPUT1_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AOUTPUT2_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AOUTPUT3_STATUS_SIZE},
      device::DInputsGroup{DINPUT_AOUTPUT4_STATUS_SIZE}
    };
    etl::array<device::DOutputsGroup, DOUTPUT_GROUP_SIZE> _doutputs_storage{
      device::DOutputsGroup{DOUTPUT_CHANNEL_SIZE}
    };
    etl::array<device::AInput,  AINPUT_CHANNEL_SIZE>  _ainputs_storage;
    etl::array<device::AOutput, AOUTPUT_CHANNEL_SIZE> _aoutputs_storage;

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
        DOUTPUTS_SIGNAL = DINPUT_GROUP_DOUTPUTS_SIGNAL,
        AINPUT1_STATUS  = DINPUT_GROUP_AINPUT1_STATUS,
        AINPUT2_STATUS  = DINPUT_GROUP_AINPUT2_STATUS,
        AINPUT3_STATUS  = DINPUT_GROUP_AINPUT3_STATUS,
        AINPUT4_STATUS  = DINPUT_GROUP_AINPUT4_STATUS,
        AINPUT5_STATUS  = DINPUT_GROUP_AINPUT5_STATUS,
        AINPUT6_STATUS  = DINPUT_GROUP_AINPUT6_STATUS,
        AINPUT7_STATUS  = DINPUT_GROUP_AINPUT7_STATUS,
        AINPUT8_STATUS  = DINPUT_GROUP_AINPUT8_STATUS,
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

    /**
     * \brief Структура списка каналов аналоговых входов прошлых версий
     * для обратной совместимости
     * \deprecated
     * \internal
     */
    struct AINPUTS
    {
      /**
       * \brief Перечисление списка каналов аналоговых входов прошлых версий
       * для обратной совместимости
       * \deprecated
       * \details
       * В новых проектах следует использовать \ref AINPUT_CHANNEL
       */
      enum NUMBER
      {
        CHANNEL1 = AINPUT_CHANNEL1,
        CHANNEL2 = AINPUT_CHANNEL2,
        CHANNEL3 = AINPUT_CHANNEL3,
        CHANNEL4 = AINPUT_CHANNEL4,
        CHANNEL5 = AINPUT_CHANNEL5,
        CHANNEL6 = AINPUT_CHANNEL6,
        CHANNEL7 = AINPUT_CHANNEL7,
        CHANNEL8 = AINPUT_CHANNEL8,
        SIZE     = AINPUT_CHANNEL_SIZE
      };
    };

    /**
     * \brief Структура списка каналов аналоговых выходов прошлых версий
     * для обратной совместимости
     * \deprecated
     * \internal
     */
    struct AOUTPUTS
    {
      /**
       * \brief Перечисление списка каналов аналоговых входов прошлых версий
       * для обратной совместимости
       * \deprecated
       * \details
       * В новых проектах следует использовать \ref AINPUT_CHANNEL
       */
      enum NUMBER : uint8_t
      {
        CHANNEL1 = AOUTPUT_CHANNEL1,
        CHANNEL2 = AOUTPUT_CHANNEL2,
        CHANNEL3 = AOUTPUT_CHANNEL3,
        CHANNEL4 = AOUTPUT_CHANNEL4,
        SIZE     = AOUTPUT_CHANNEL_SIZE
      };
    };
  };

}