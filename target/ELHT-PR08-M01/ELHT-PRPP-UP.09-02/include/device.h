/**
 * \file device.h
 * \brief Библиотека приборов
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <cstdint>
#include "menu.h"
#include "device/dio.h"
#include "device/aio.h"
#include "etl/string.h"
#include "etl/to_string.h"

namespace device
{
  //! Общие параметры настроек устройств
  namespace parameter
  {
    //! Период опроса в мс
    inline const menu::var<uint16_t> poll_period{{
      .default_value {10}
    }};
    //! Флаг непрерывной записи в устройство при автоматическом опросе
    inline const menu::var<bool> auto_write{{
      .default_value {false}
    }};
  }

  //! Канал опроса устройства
  struct PollChannel
  {
  protected:
    struct Init
    {
      menu::iTableTree& tree;
      uint32_t poll_period {10};
      bool     auto_write  {false};
    };

    /**
     * \brief Записывает в настройки общие параметры
     * \details
     * Обязательно вызывать после инициализации таблицы параметров с настройками
     * в классе-наследнике.
     * 
     * \note На момент вызова базового конструктора скорее всего таблица 
     * параметров еще не создана, и вызов данных методов приведет к HardFault'у
     */
    void __setInitSettings(const Init& init)
    {
      settings.set(parameter::poll_period, init.poll_period);
      settings.set(parameter::auto_write,  init.auto_write);
    }

  public:
    enum
    {
      CONNECTION_QUALITY_MAX = 100
    };

    //! Дерево таблиц параметров для опроса
    menu::iTableTree& tree;
    //! Настройки опроса
    menu::iTable& settings;
    //! Флаг ошибки связи с каналом опроса
    bool failed_connection{};
    //! Флаг обновления чтения данных в канале опроса
    bool is_updated_read{};
    //! Флаг обновления записи данных в канале опроса
    bool is_updated_write{};
    //! Качество связи
    uint8_t connection_quality{};
    //! Кол-во хороших посылок для определения качества связи
    uint8_t connection_quality_good_packets{};
    //! Общее кол-во посылок для определения качества связи
    uint8_t connection_quality_total_packets{};
    //! Время последнего опроса
    size_t last_poll_time{};
    //! Кол-во пропусков группы опроса при попытке чтения
    uint32_t try_poll_fails{};

    PollChannel(const Init& init, menu::iTable& settings)
    : tree     {init.tree},
      settings {settings}
    {
    }

    /**
     * \brief Информирует канал опроса о успешной связи
     * \param readed Флаг успешного чтения канала опроса
     * \param writed Флаг успешной записи канала опроса
     */
    void communicationSuccess(bool readed, bool writed);

    /**
     * \brief Информирует канал опроса об ошибке связи
     */
    void communicationFail();
  };


  /**
   * \brief Информация об устройстве
   */
  struct DeviceInfo
  {
    //! Название устройства
    etl::string_view name{"Device"};
    //! Считанный код модификации устройства
    etl::string_view modification{"UNKNOWN"};
    //! Считанный серийный номер устройства
    etl::string_view serial_number{"UNKNOWN"};
    //! Мажорная версия устройства (согласно семантическому версионированию)
    uint8_t version_major{};
    //! Минорная версия устройства (согласно семантическому версионированию)
    uint8_t version_minor{};
    //! Патч версия устройства (согласно семантическому версионированию)
    uint8_t version_patch{};
  };


  //! Интерфейс устройства
  class iDevice
  {
  public:
    //! Тип переменной состояния устройства
    using state_t = uint8_t;

    //! Состояния устройства
    enum STATE : state_t
    {
      STATE_RESET = 0, //!< Устройство не инициализировано
      STATE_READY = 1, //!< Устройство готово к работе
      STATE_SIZE  = 2, //!< Кол-во стандартных состояний устройства
    };

  protected:
    struct Init {
      //! Каналы опроса регистров прибора
      const etl::span<PollChannel* const> poll_channels;
      //! Дискретные входы
      const etl::span<DInputsGroup> dinputs;
      //! Дискретные выходы
      const etl::span<DOutputsGroup> doutputs;
      //! Аналоговые входы
      const etl::span<AInput> ainputs;
      //! Аналоговые выходы
      const etl::span<AOutput> aoutputs;
    };

    //! Состояние устройства
    state_t _state{STATE_RESET};

  public:
    //! Каналы опроса регистров прибора
    const etl::span<PollChannel* const> poll_channels;
    //! Дискретные входы
    const etl::span<DInputsGroup>  dinputs;
    //! Дискретные выходы
    const etl::span<DOutputsGroup> doutputs;
    //! Аналоговые входы
    const etl::span<AInput> ainputs;
    //! Аналоговые выходы
    const etl::span<AOutput> aoutputs;
    //! Информация об устройстве
    DeviceInfo device_info{};
    

    /**
     * \brief Возвращает состояние устройства
     */
    state_t getState() const { return _state; }

    /**
     * \brief Сбрасывает устройство в состояние STATE_RESET
     * \details
     * Позволяет выполнить инициализацию устройства заново
     */
    void reset();

    /**
     * \brief Инициализирует устройство
     * \param[in] node Корневой узел дерева параметров, в который можно
     * записывать новые данные для инициализации
     * \details
     * Метод может модифицировать каналы опроса в устройстве чтобы, узнать
     * информацию и записать ее в \ref device_info.
     * 
     * Процесс инициализации считается продолжающимся до тех пор, пока
     * состояние устройства не станет \ref STATE_READY, т.е. метод является
     * неблокирующим.
     * 
     * \note В реализации устройства могут быть дополнительные состояния для 
     * инициализации, но после окончания всех процессов состояние обязательно
     * должно стать \ref STATE_READY
     * 
     * \return Канал опроса, который должен быть выполнен для инициализации
     * \retval nullptr Для текущего этапа инициализации выполнение канала
     * опроса не требуется
     */
    virtual PollChannel* init(menu::iTableNode* node);

    /**
     * \brief Обновляет входы/выходы перед опросом
     * \details
     * \note Вызывать перед считыванием канала опроса
     */
    virtual void scan(PollChannel* next_channel);

    /**
     * \brief Обновляет входы/выходы после опроса
     * \details
     * \note Вызывать после считывания канала опроса
     * с входами/выходами 
     */
    virtual void update();

  protected:
    iDevice(const Init& init)
    : poll_channels {init.poll_channels},
      dinputs       {init.dinputs},
      doutputs      {init.doutputs},
      ainputs       {init.ainputs},
      aoutputs      {init.aoutputs}
    {

    }
  };
}

#endif /* DEVICE_H */
