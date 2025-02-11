/**
 * \file modbus/devices/elhart/alphax_module_modifications.h
 * \brief Список модификаций модулей Alpha-X
 */

#pragma once

#include <cstdint>
#include "etl/string_view.h"

namespace modbus::devices::elhart
{
  /**
   * \brief Описание модификации Alpha-X DI16
   */
  struct AlphaX_DI16_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name = "Alpha-X DI 16";
    static inline const etl::string_view modification = "50201";
    using discrete_inputs  = etl::integral_constant<uint8_t, 16>;
    using discrete_outputs = etl::integral_constant<uint8_t, 0>;
    using analog_inputs    = etl::integral_constant<uint8_t, 0>;
    using analog_outputs   = etl::integral_constant<uint8_t, 0>;
  };

  /**
   * \brief Описание модификации Alpha-X DO 16P
   */
  struct AlphaX_DO16P_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name = "Alpha-X DO 16P";
    static inline const etl::string_view modification = "50202";
    using discrete_inputs  = etl::integral_constant<uint8_t, 0>;
    using discrete_outputs = etl::integral_constant<uint8_t, 16>;
    using analog_inputs    = etl::integral_constant<uint8_t, 0>;
    using analog_outputs   = etl::integral_constant<uint8_t, 0>;
  };

  /**
   * \brief Описание модификации Alpha-X DO 12R
   */
  struct AlphaX_DO12R_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name = "Alpha-X DO 12R";
    static inline const etl::string_view modification = "50203";
    using discrete_inputs  = etl::integral_constant<uint8_t, 0>;
    using discrete_outputs = etl::integral_constant<uint8_t, 12>;
    using analog_inputs    = etl::integral_constant<uint8_t, 0>;
    using analog_outputs   = etl::integral_constant<uint8_t, 0>;
  };

  /**
   * \brief Описание модификации Alpha-X DIO 8/8P
   */
  struct AlphaX_DIO8_8P_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name = "Alpha-X DIO 8/8P";
    static inline const etl::string_view modification = "50204";
    using discrete_inputs  = etl::integral_constant<uint8_t, 8>;
    using discrete_outputs = etl::integral_constant<uint8_t, 8>;
    using analog_inputs    = etl::integral_constant<uint8_t, 0>;
    using analog_outputs   = etl::integral_constant<uint8_t, 0>;
  };

  /**
   * \brief Описание модификации Alpha-X DIO 8/6R
   */
  struct AlphaX_DIO8_6R_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name= "Alpha-X DIO 8/6R";
    static inline const etl::string_view modification = "50205";
    using discrete_inputs  = etl::integral_constant<uint8_t, 8>;
    using discrete_outputs = etl::integral_constant<uint8_t, 6>;
    using analog_inputs    = etl::integral_constant<uint8_t, 0>;
    using analog_outputs   = etl::integral_constant<uint8_t, 0>;
  };

  /**
   * \brief Описание модификации Alpha-X AI 8
   */
  struct AlphaX_AI8_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name = "Alpha-X AI 8";
    static inline const etl::string_view modification = "50301";
    using discrete_inputs  = etl::integral_constant<uint8_t, 0>;
    using discrete_outputs = etl::integral_constant<uint8_t, 0>;
    using analog_inputs    = etl::integral_constant<uint8_t, 8>;
    using analog_outputs   = etl::integral_constant<uint8_t, 0>;
  };

  /**
   * \brief Описание модификации Alpha-X AI 6T
   */
  struct AlphaX_AI6T_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name = "Alpha-X AI 6T";
    static inline const etl::string_view modification = "50302";
    using discrete_inputs  = etl::integral_constant<uint8_t, 0>;
    using discrete_outputs = etl::integral_constant<uint8_t, 0>;
    using analog_inputs    = etl::integral_constant<uint8_t, 6>;
    using analog_outputs   = etl::integral_constant<uint8_t, 0>;
  };

  /**
   * \brief Описание модификации Alpha-X AO 4
   */
  struct AlphaX_AO4_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name = "Alpha-X AO 4";
    static inline const etl::string_view modification = "50303";
    using discrete_inputs  = etl::integral_constant<uint8_t, 0>;
    using discrete_outputs = etl::integral_constant<uint8_t, 0>;
    using analog_inputs    = etl::integral_constant<uint8_t, 0>;
    using analog_outputs   = etl::integral_constant<uint8_t, 4>;
  };

  /**
   * \brief Описание модификации Alpha-X DAIO 5/5P 2T/0
   */
  struct AlphaX_DAIO5_5P_2T_0_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name = "Alpha-X DAIO 5/5P 2T/0";
    static inline const etl::string_view modification = "50401";
    using discrete_inputs  = etl::integral_constant<uint8_t, 5>;
    using discrete_outputs = etl::integral_constant<uint8_t, 5>;
    using analog_inputs    = etl::integral_constant<uint8_t, 2>;
    using analog_outputs   = etl::integral_constant<uint8_t, 0>;
  };

  /**
   * \brief Описание модификации Alpha-X DAIO 8/4R 2/0
   */
  struct AlphaX_DAIO8_4R_2_0_Info : public DeviceModificationInfo
  {
    static inline const etl::string_view name = "Alpha-X DAIO 8/4R 2/0";
    static inline const etl::string_view modification = "50402";
    using discrete_inputs  = etl::integral_constant<uint8_t, 8>;
    using discrete_outputs = etl::integral_constant<uint8_t, 4>;
    using analog_inputs    = etl::integral_constant<uint8_t, 2>;
    using analog_outputs   = etl::integral_constant<uint8_t, 0>;
  };


  /**
   * \brief Список модификаций модулей Alpha-X DI/DO/DIO (дискретного
   * ввода-вывода)
   */
  extern const etl::array<DeviceModificationInfo, 5> alphax_dio_mod_info;

  /**
   * \brief Список модификаций модулей Alpha-X AI/AO/AIO (аналогового
   * ввода-вывода)
   */
  extern const etl::array<DeviceModificationInfo, 3> alphax_aio_mod_info;

  /**
   * \brief Список модификаций модулей Alpha-X DAIO (комбинированный)
   */
  extern const etl::array<DeviceModificationInfo, 2> alphax_daio_mod_info;
}
