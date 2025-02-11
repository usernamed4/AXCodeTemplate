/**
 * \file kernel/kernel_api_state.h
 * \brief API состояний ядра
 * \ingroup kernel_api_state
 */

#pragma once

#include "kernel/kernel_base.h"

namespace kernel
{
  /**
   * \addtogroup kernel_api_state
   * \{
   */

  /**
   * \brief Флаги состояний ядра
   */
  struct KERNEL_STATE
  {
    //! Тип данных перечисления состояний ядра
    using type = uint16_t;

    //! Битовая маска состояний ядра
    enum BITMASK : type
    {
      RESET    = 0x00, //!< Ядро не запущено
      INIT     = 0x01, //!< Инициализация задач
      RUN      = 0x02, //!< Рабочий режим
      STOP     = 0x04, //!< Остановка задач
      SERVICE  = 0x08, //!< Сервисный режим
      SHUTDOWN = 0x10, //!< Выключение ядра по команде
      POWEROFF = 0x20, //!< Выключение питания

      ERROR    = 0x40, //!< Ошибка определения состояния ядра
      ALL      = 0x3F  //!< Маска всех возможных состояний ядра
    };
  };

  /**
   * \}
   * \noop kernel_api_state
   */
}