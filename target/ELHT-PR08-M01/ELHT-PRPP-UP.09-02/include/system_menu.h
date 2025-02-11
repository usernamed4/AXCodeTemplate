/**
 * \file system_menu.h
 * \brief Системная таблица параметров Alpha-X CPU
 */

#ifndef SYSTEM_MENU_H
#define SYSTEM_MENU_H

#include "kernel/kernel_api_state.h"
#include "menu.h"
#include "modification_menu.h"

namespace cpu
{
  //! Пространство имен параметров системного меню
  namespace parameter
  {
    //! Номер модификации процессорного модуля
    inline const menu::var<uint16_t> serial_modification{{
      .flags {menu::flag::VISIBLE}
    }};

    //! Серийный номер процессорного модуля
    inline const menu::var<uint16_t> serial_number{{
      .flags {menu::flag::VISIBLE}
    }};

    //! Major версия ядра (v1.X.X)
    inline const menu::var<uint16_t> version_major{{
      .flags {menu::flag::VISIBLE}
    }};

    //! Minor версия ядра (vX.1.X)
    inline const menu::var<uint16_t> version_minor{{
      .flags {menu::flag::VISIBLE}
    }};

    //! Patch версия ядра (vX.X.1)
    inline const menu::var<uint16_t> version_patch{{
      .flags {menu::flag::VISIBLE}
    }};

    //! Состояние встроенных дискретных входов Alpha-X CPU
    inline const menu::var<uint16_t> dinputs{{
      .flags {menu::flag::VISIBLE}
    }};

    //! Состояние встроенных дискретных выходов Alpha-X CPU
    inline const menu::var<uint16_t> doutputs{{
      .flags {menu::flag::VISIBLE}
    }};

    //! Счетчик ошибок в логе ошибок
    inline const menu::var<uint16_t> log_errors_count{{
      .flags {menu::flag::VISIBLE}
    }};

    /**
     * \brief Состояние ядра
     * \details
     * Возможные значения: \ref kernel::KERNEL_STATE::BITMASK
     */
    inline const menu::var<uint16_t> kernel_state
    {{
      .flags         {menu::flag::VISIBLE},
      .default_value {kernel::KERNEL_STATE::RESET},
      .min_value     {kernel::KERNEL_STATE::RESET},
      .max_value     {kernel::KERNEL_STATE::ALL}
    }};

    //! Значения переменной переключения состояния ядра
    struct KERNEL_ACTION
    {
      using type = uint16_t;

      enum VALUE : type
      {
        NONE              = 0, //!< Нет ожидаемого действия
        RUN               = 1, //!< Переключить в RUN
        STOP              = 2, //!< Переключить в STOP
        RELOAD            = 3, //!< Перезагрузить модуль
        RELOAD_SERVICE    = 4, //!< Перезагрузиться в сервисный режим
        RELOAD_BOOTLOADER = 5,  //!< Перезагрузиться в загрузчик

        /**
         * \brief Перезагрузиться в сервисный режим и включить все порты
         * в режиме Modbus-конвертера с адресом 100
         * \details
         * ПЛК будет пересылать все Modbus-запросы с портов на COM1, кроме
         * запросов с адресом 100 (собственный адрес)
         * \todo Параметр может быть убран в следующих версиях (заменен
         * на более детальную настройку режима), рекомендуется использование
         * только в ручном режиме настройки устройств на шине COM1
         */
        RELOAD_SERVICE_CONVERTER = 100
      };
    };

    /**
     * \brief Переключение состояния ядра
     * \details
     * Возможные значения: \ref KERNEL_ACTION::VALUE
     */
    inline const menu::var<uint16_t> kernel_action{{
      .default_value {KERNEL_ACTION::NONE}
    }};


    //! Значения переменной сброса retain
    struct RESET_RETAIN
    {
      enum VALUE : uint16_t
      {
        NONE     = 0,     //!< Нет действия
        ACCEPTED = 100,   //!< Команда на сброс retain принята
        RESETTED = 200,   //!< Retain сброшен
        RESET    = 0xD5BF //!< Сбросить retain
      };
    };

    /**
     * \brief Сброс retain на значения по умолчанию
     * \details
     * Возможные значения: \ref RESET_RETAIN::VALUE
     */
    inline const menu::var<uint16_t> reset_retain;
  }

  //! Описание системного меню Alpha-X CPU
  inline const menu::TableInfo system_menu_info{
    parameter::serial_modification,
    parameter::serial_number,
    parameter::version_major,
    parameter::version_minor,
    parameter::version_patch,
    parameter::dinputs,
    parameter::doutputs,
    parameter::log_errors_count,
    parameter::kernel_state,
    parameter::kernel_action,
    parameter::reset_retain
  };

  //! Тип переменной системной таблицы параметров
  using SystemTable = decltype(menu::Table{system_menu_info});

  //! Мьютекс доступа к системной таблице параметров
  inline elh::mutex system_menu_mutex;

  //! Системная таблица параметров Alpha-X CPU
  inline SystemTable system_menu{system_menu_info, &system_menu_mutex};

}

#endif /* SYSTEM_MENU_H */
