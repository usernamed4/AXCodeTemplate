/**
 * \file modbus_base.h
 * \brief Общие определения библиотеки Modbus 
 */

#ifndef MODBUS_BASE_H
#define MODBUS_BASE_H

#include <cstdint>
#include "etl/vector.h"
#include "etl/variant_pool.h"
#include "etl/byte_stream.h"
#include "etl/delegate.h"

namespace modbus
{
  //! Перечисления с константами посылки Modbus
  enum PACKET
  {
    REGISTER_SIZE = 2,
    CRC_SIZE      = 2
  };

  //! Перечисление адресов Modbus
  enum ADDRESS
  {
    BROADCAST_ADDRESS = 0,   //!< Широковещательный адрес
    MIN_ADDRESS       = 1,   //!< Минимально возможный адрес устройства
    MAX_ADDRESS       = 247  //!< Максимально возможный адрес устройства
  };

  //! Список ошибок запросов Modbus
  enum EXCEPTION_CODE
  {
    EXCEPTION_NONE                  = 0x00,
    EXCEPTION_ILLEGAL_FUNCTION      = 0x01,
    EXCEPTION_ILLEGAL_DATA_ADDRESS  = 0x02,
    EXCEPTION_ILLEGAL_DATA_VALUE    = 0x03,
    EXCEPTION_SERVER_DEVICE_FAILURE = 0x04,

    // Расширенный список ошибок Modbus
    EXCEPTION_MASTER_REQUEST_ERROR  = 97,  //!< Ошибка запроса Modbus Master
    EXCEPTION_UNKNOWN_ANSWER        = 98,  //!< Неизвестный ответ
    EXCEPTION_DEVICE_NOT_RESPONDING = 99   //!< Устройство не отвечает
  };


  namespace function
  {
    //! Общая для функций структура посылки
    enum PACKET
    {
      PACKET_DEVICE_ADDRESS = 0, //!< Адрес устройства в сети
      PACKET_FUNCTION_CODE  = 1  //1< Код функции Modbus
    };

    //! Описание посылки ошибки Modbus
    enum PACKET_EXCEPTION
    {
      PACKET_EXCEPTION_CODE = 2, //<! Положение кода ошибки в посылке ответа
      PACKET_EXCEPTION_SIZE = 3  //<! Размер посылки ответа с ошибкой без CRC
    };
  }

  /**
   * \brief Рассчитывает интервал тишины между посылками
   * \param baudrate Скорость обмена, бит/с
   * \param high_speed Флаг расчета интервала для скоростей больше 19200
   * \details
   * Интервал тишины составляет 3,5 передаваемых слова.  
   * Для скоростей больше 19200 устанавливается интервал 1750 мкс.
   * 
   * Считает по формуле y = 38.5 / x, где
   *    38.5 - 3.5 символа при длине символа 11 бит по стандарту,
   *    x    - скорость обмена, бит/сек
   * 
   * \return Интервал тишины в мкс
   */
  uint32_t ComputeSilentInterval(uint32_t baudrate, bool high_speed=false);
}

#endif /* MODBUS_BASE_H */