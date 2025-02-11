/**
 * \file plc/fb_device.h
 * \brief Функциональные блоки работы с устройствами
 */

#pragma once

#include "plc/fb_base.h"
#include "plc/private/fb_device_private.h"


namespace plc
{
  /**
   * \brief Список функциональных блоков настройки устройства
   */
  template<class DEVICE_T>
  struct DeviceSetup
  {
    //! Функциональный блок настройки дискретных входов
    using DiscreteInputs     = void;
    //! Функциональный блок чтения настроек дискретных входов
    using ReadDiscreteInputs = void;

    //! Функциональный блок настройки дискретных выходов
    using DiscreteOutputs     = void;
    //! Функциональный блок чтения настроек дискретных выходов
    using ReadDiscreteOutputs = void;

    //! Функциональный блок настройки аналоговых входов
    using AnalogInputs     = void;
    //! Функциональный блок чтения настроек аналоговых входов
    using ReadAnalogInputs = void;

    //! Функциональный блок настройки аналоговых выходов
    using AnalogOutputs     = void;
    //! Функциональный блок чтения настроек аналоговых выходов
    using ReadAnalogOutputs = void;

    //! Функциональный блок сброса на заводские настройки
    using FactoryReset   = void;
    //! Функциональный блок сохранения параметров в энергонезависимую память
    using SaveParameters = void;
  };

  /**
   * \brief Входные параметры ФБ \ref DevicePollChannelNumberStatus
   */
  struct DevicePollChannelNumberStatus_Input
  {
    device::iDevice* device  {nullptr}; //!< Устройство
    size_t           channel {0};       //!< Номер канала опроса
  };


  /**
   * \brief Считывает состояние канала опроса в устройстве по номеру канала
   * \details
   * Блок выполняется мгновенно после вызова
   */
  class DevicePollChannelNumberStatus
  : public DevicePollChannelNumberStatus_Input,
    public LConC,
    private __DevicePollChannelStatus_Private
  {
  public:
    //! Структура входных параметров функционального блока
    using Input = DevicePollChannelNumberStatus_Input;

    //! Связь с каналом опроса отсутствует
    bool failed_connection() const;
    //! Качество связи с каналом опроса (0...100%)
    uint32_t connection_quality() const;
    //! Время последнего опроса (мс)
    uint32_t last_poll_time() const;

    DevicePollChannelNumberStatus(const Input& init);

  protected:
    FB_STATE processExecuting() override;
  };


  /**
   * \brief Входные параметры ФБ считывания состояния канала опроса
   */
  struct DevicePollChannelStatus_Input
  {
    device::PollChannel* poll_channel{nullptr}; //!< Канал опроса
  };


  /**
   * \brief Считывает состояние канала опроса
   */
  class DevicePollChannelStatus
  : public DevicePollChannelStatus_Input,
    public LConC,
    private __DevicePollChannelStatus_Private
  {
  public:
    //! Структура входных параметров функционального блока
    using Input = DevicePollChannelStatus_Input;

    //! Связь с каналом опроса отсутствует
    bool failed_connection() const;
    //! Качество связи с каналом опроса (0...100%)
    uint32_t connection_quality() const;
    //! Время последнего опроса (мс)
    uint32_t last_poll_time() const;

    DevicePollChannelStatus(const Input& init);

  private:
    FB_STATE processExecuting() override;
  };
}
