/**
 * \file plc/private/fb_device_private.h
 * \brief Внутренние определения фунциональных блоков работы с устройствами
 * \details
 * Список внутренних определений для функциональных блоков работы
 * с устройствами, которые должны быть видимы в заголовочных файлах API
 */

#pragma once

#include "plc/fb_base.h"


namespace plc
{
  /**
   * \brief Шаблон массива каналов устройства
   * \tparam T Класс канала
   * \tparam N Кол-во каналов
   * \details
   * Используется для быстрого создания массива каналов, привязанных к
   * определенному устройству
   */
  template<typename T, size_t N>
  class ChannelArray : public etl::array<T, N>
  {
  public:
    using base_class = etl::array<T, N>;

    /**
     * \brief Конструктор массива каналов устройства
     * \param device Устройство с каналами
     * \param start_channel Номер первого канала массива
     * \details
     * Создаст массив каналов от start_channel до start_channel + N.
    */
    constexpr ChannelArray(device::iDevice& device, uint16_t start_channel=0)
    : base_class{make_array(
        device, start_channel, etl::make_index_sequence<N>()
      )}
    {
    }

    /**
     * \brief Конструктор массива каналов по указателю на устройство
     * \param device Устройство с каналами
     * \param start_channel Номер первого канала массива
     * \details
     * Создаст массив каналов от start_channel до start_channel + N.
    */
    constexpr ChannelArray(device::iDevice* device, uint16_t start_channel=0)
    : ChannelArray(*device, start_channel)
    {
    }

    //! Нулевой указатель на устройство в конструкторе запрещен
    ChannelArray(nullptr_t, uint8_t) =delete;

  private:
    /**
     * \brief Конструктор массива и его элементов
     * \tparam IS index_sequence размера массива
     * \param device Устройство с каналами
     * \param start_channel Номер первого канала массива
     * \param args Дополнительные аргументы конструктора канала
     */
    template<size_t... IS>
    constexpr auto make_array(
      device::iDevice& device,
      uint8_t start_channel,
      const etl::index_sequence<IS...>&
    ) noexcept -> base_class
    {
      return {T{device, static_cast<uint8_t>(start_channel + IS)}...};
    }
  };


  /**
   * \brief Список общих внутренних переменных ФБ чтения статуса каналов опроса
   */
  struct __DevicePollChannelStatus_Private
  {
    bool     _failed_connection{};
    uint32_t _connection_quality{};
    uint32_t _last_poll_time{};
  };
}
