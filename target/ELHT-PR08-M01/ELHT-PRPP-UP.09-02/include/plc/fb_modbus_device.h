/**
 * \file fb_modbus_device.h
 * \brief Устройство Modbus библиотеки функциональных блоков Modbus
 * \ingroup plc_com_modbus
 */

#pragma once

#include "plc/fb_device.h"
#include "plc/fb_modbus_buffer.h"
#include "modbus/modbus_device.h"


namespace plc
{
  /**
   * \addtogroup plc_com_modbus
   * \{
   */

  /**
   * \brief Устройство Modbus с одним каналом опроса
   */
  struct ModbusFBDevice : public modbus::Device
  {
    iModbusBufferBase* buffer;
    menu::TableNode<1> register_node;
    modbus::ModbusTree<1> register_tree{register_node};
    modbus::ModbusPollChannel poll_channel;
    etl::array<device::PollChannel* const, 1> poll_channels_storage {
      &poll_channel
    };

    //! Структура инициализации устройства
    struct Init {
      //! Адрес устройства Modbus
      uint8_t  device_address {1};
      //! Период опроса, мс (0 - опрос выключен)
      uint32_t poll_period    {10};
      //! Флаг постоянной записи в устройство (false - постоянное чтение)
      bool     auto_write     {false};
      //! Modbus функция записи
      uint8_t  function_write {16};
      //! Modbus функция чтения
      uint8_t  function_read  {3};
      //! Первый адрес регистра Modbus в канале опроса
      uint16_t register_start_address {0};
      //! Максимальное кол-во регистров в одном запросе Modbus (0 - стандарт)
      uint8_t  request_registers      {0};
    };

    /**
     * \brief Конструктор устройства Modbus с одним каналом опроса
     * \param init   Структура инициализации устройства
     * \param buffer Указатель на буфер Modbus
     */
    ModbusFBDevice(const Init& init, iModbusBufferBase* buffer);

    /**
     * \brief Конструктор устройства Modbus по ссылке на буфер
     * \param device_address Адрес устройства в сети Modbus
     * \param register_start_address Первый адрес регистра Modbus в канале
     * опроса
     * \param buffer Ссылка на буфер Modbus
     */
    ModbusFBDevice(
      uint8_t device_address,
      uint16_t register_start_address,
      iModbusBufferBase& buffer
    );

    /**
     * \brief Конструктор устройства Modbus по указателю на буфер
     * \param device_address Адрес устройства в сети Modbus
     * \param register_start_address Первый адрес регистра Modbus в канале
     * опроса
     * \param buffer Указатель на буфер Modbus
     */
    ModbusFBDevice(
      uint8_t device_address,
      uint16_t register_start_address,
      iModbusBufferBase* buffer
    );

    ModbusFBDevice(uint8_t, uint16_t, nullptr_t) =delete;
  };


  /**
   * \brief Устройство автоматического канала опроса буфера Modbus
   * \tparam SIZE Размер буфера Modbus
   * \details
   * Используется для создания автоматически опрашиваемого буфера Modbus.
   * Работа с буфером осуществляется через методы \ref iModbusBuffer.
   * 
   * Устройство необходимо добавить в список \ref ModbusMasterPoll,
   * используемый в функциональном блоке \ref ModbusMaster
   */
  template<size_t N>
  class ModbusBufferDevice : public ModbusBuffer<N>, public ModbusFBDevice
  {
  public:
    //! Тип структуры инициализации устройства
    using Init = ModbusFBDevice::Init;

    /**
     * \brief Конструктор устройства
     * \param init Структура инициализации устройства
     */
    ModbusBufferDevice(
      const Init& init={.function_write{16}, .function_read{3}}
    )
    : ModbusBuffer<N>{},
      ModbusFBDevice{init, static_cast<iModbusBufferBase*>(this)}
    {
    }
  };

  
  /**
   * \brief Устройства автоматического канала опроса буфера Coils Modbus
   * \tparam SIZE Размер буфера Coils Modbus
   * \details
   * Используется для создания автоматически опрашиваемого буфера Modbus.
   * Работа с буфером осуществляется через методы \ref iModbusCoilBuffer.
   * 
   * Устройство необходимо добавить в список \ref ModbusMasterPoll,
   * используемый в функциональном блоке \ref ModbusMaster
   */
  template<size_t N>
  class ModbusCoilBufferDevice
  : public ModbusCoilBuffer<N>, public ModbusFBDevice
  {
  public:
    //! Тип структуры инициализации устройства
    using Init = ModbusFBDevice::Init;

     /**
     * \brief Конструктор устройства
     * \param init Структура инициализации устройства
     */
    ModbusCoilBufferDevice(
      const Init& init={.function_write {5}, .function_read{1}}
    )
    : ModbusCoilBuffer<N>{},
      ModbusFBDevice{init, static_cast<iModbusBufferBase*>(this)}
    {
    }
  };

  /**
   * \}
   * \noop
   */
}
