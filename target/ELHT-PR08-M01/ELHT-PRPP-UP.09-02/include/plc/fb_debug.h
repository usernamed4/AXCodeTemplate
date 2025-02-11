/**
 * \file plc/fb_debug.h
 * \brief Функциональные блоки и функции отладки
 * \ingroup plc_com_debug
 */

#pragma once

#include "fb_base.h"
#include "fb_serial_port.h"
#include "kernel/kernel_api_debug.h"

namespace plc
{
  /**
   * \addtogroup plc_com_debug
   * \{
   */

  /**
   * \brief Интерфейс ФБ настройки порта отладки
   * \details
   * При создании ФБ создается отладка на выбранном порте.
   * Для создания ФБ используется класс \ref DebugPort
   * 
   * Требуется чтобы настроить порт:
   * - На отправку отладочных сообщений функциями \ref print_debug(),
   * \ref print_event() и \ref print_error()
   * - На прием отладочных сообщений функцией \ref scan_debug()
   */
  class iDebugPort : public ETrigATo, protected component_link
  {
  public:
    //! Номер COM-порта
    const COM com;

    //! Скорость обмена, бит/с
    uint32_t baudrate;
    //! Четность
    COM_PARITY parity;
    //! Кол-во стоп-битов
    uint8_t stop_bits;
    //! Отправлять отладочные сообщения
    bool debug_messages;
    //! Отправлять сообщения событий
    bool event_messages;
    //! Отправлять сообщения ошибок
    bool error_messages;

  protected:
    //! Структура инициализации функционального блока
    struct Init
    {
      //! Номер COM-порта
      const COM com;

      //! Скорость обмена, бит/с
      uint32_t baudrate {115200};
      //! Четность
      COM_PARITY parity {COM_PARITY_NONE};
      //! Кол-во стоп-битов
      uint8_t stop_bits {1};
      //! Отправлять отладочные сообщения
      bool debug_messages {true};
      //! Отправлять сообщения событий
      bool event_messages {true};
      //! Отправлять сообщения ошибок
      bool error_messages {true};
    };

  protected:
    iDebugPort(const Init& init, kernel::debug::iDebugBuffer& buffer);

  private:
    inline static component_list<iDebugPort> _list;
    TaskHandle_t _task;
    kernel::debug::iDebugBuffer& _buffer;

    virtual FB_STATE processExecuting() override final;

    friend component_list<iDebugPort>;
    friend class ComponentListHandler;
  };


  /**
   * \brief Функциональный блок  порта отладки с изменяемым размером буфера
   * отладки
   * \tparam N Размер буфера отладки в байтах
   */
  template<size_t N=1024>
  class DebugPort : public iDebugPort
  {
  public:
    /**
     * \brief Конструктор ФБ порта отладки
     * \param init Структура инициализации входных параметров
     * \details
     * Структуру инициализации нужно поместить в фигурные скобки в аргументы
     * конструктора:
     * \code{.cpp}
     * DebugPort debug_port({
     *   .com        = COM1,
     *   .baudrate   = 115200,
     *   .parity     = COM_PARITY_NONE,
     *   .stop_bits  = 1,
     *   .debug_messages = true,
     *   .event_messages = true,
     *   .error_messages = true
     * });
     * \endcode
     * 
     * По умолчанию размер буфера отладки - 1024 байта. Для изменения размера
     * буфера нужно указать новый размер в байтах в треугольных скобках после 
     * названия класса.
     * \code{.cpp}
     * DebugPort<2048> debug_port({
     * \endcode
     * 
     * Если в структуре инициализации не будет указан один из аргументов
     * (например, baudrate), то он примет значение по умолчанию из объявления
     * структуры Init.
     * 
     * Параметры конструктора должны идти в порядке, указанном в объявлении
     * структуры инициализации Init.
     */
    DebugPort(const Init& init)
    : iDebugPort{init, _buffer_storage},
      _buffer_storage{kernel::debug::print_type_t(
        ((debug_messages) ? kernel::debug::PRINT_TYPE_DEBUG : 0) |
        ((event_messages) ? kernel::debug::PRINT_TYPE_EVENT : 0) |
        ((error_messages) ? kernel::debug::PRINT_TYPE_ERROR : 0)
      )}
    {
    }
  
  private:
    kernel::debug::DebugBuffer<N> _buffer_storage;
  };


  /**
   * \brief Отправляет отладочное сообщение
   * \param[in] from Отправляемая строка
   * \param[in] args Аргументы для форматирования строки аналогично printf().
   * Кол-во аргументов
   * должно совпадать с кол-вли спецификаторов формата в строке from.  
   * Могут отсутствовать, если не требуются.
   * \tparam T Типы данных для форматирования строки. Подставляются
   * компилятором автоматически. Кол-во совпадает с кол-вом аргументов args.
   * \details
   * Сообщение будет отправлено на все порты, настроенные на отладку, и для
   * которых разрешено получение отладочных сообщений.
   * 
   * Если в функции указаны аргументы args, строка будет отформатирована
   * аналогично функции printf(). Размер итоговой строки при этом ограничен
   * 128 символами.
   * 
   * \note Функция не отправляет символы перевода на новую строку. Для
   * перевода необходимо отправить "\r\n"
   * 
   * Для работы функции необходимо создать ФБ \ref DebugPort.
   */
  template<typename... T>
  void print_debug(const etl::string_view& from, T... args)
  {
    return kernel::debug::print_debug(from, args...);
  }


  /**
   * \brief Отправляет сообщение события
   * \param[in] from Отправляемая строка
   * \param[in] args Аргументы для форматирования строки аналогично printf().
   * Кол-во аргументов
   * должно совпадать с кол-вли спецификаторов формата в строке from.  
   * Могут отсутствовать, если не требуются.
   * \tparam T Типы данных для форматирования строки. Подставляются
   * компилятором автоматически. Кол-во совпадает с кол-вом аргументов args.
   * \details
   * Сообщение события будет отправлено на все порты, настроенные на отладку,
   * и для которых разрешено получение событий.
   * 
   * Если в функции указаны аргументы args, строка будет отформатирована
   * аналогично функции printf(). Размер итоговой строки при этом ограничен
   * 128 символами.
   * 
   * Формат сообщения:
   * \verbatim Время дата: [EVENT] Сообщение \r\n \endverbatim
   * 
   * Для работы функции необходимо создать ФБ \ref DebugPort.
   */
  template<typename... T>
  void print_event(const etl::string_view& from, T... args)
  {
    return kernel::debug::print_event(from, args...);
  }


  /**
   * \brief Отправляет сообщение ошибки
   * \param[in] from Отправляемая строка
   * \param[in] args Аргументы для форматирования строки аналогично printf().
   * Кол-во аргументов
   * должно совпадать с кол-вли спецификаторов формата в строке from.  
   * Могут отсутствовать, если не требуются.
   * \tparam T Типы данных для форматирования строки. Подставляются
   * компилятором автоматически. Кол-во совпадает с кол-вом аргументов args.
   * \details
   * Сообщение ошибки будет отправлено на все порты, настроенные на отладку,
   * и для которых разрешено получение сообщений ошибки.
   * 
   * Если в функции указаны аргументы args, строка будет отформатирована
   * аналогично функции printf(). Размер итоговой строки при этом ограничен
   * 128 символами.
   * 
   * Формат сообщения:
   * \verbatim Время дата: [ERROR] Сообщение \r\n \endverbatim
   * 
   * Для работы функции необходимо создать ФБ \ref DebugPort.
   */
  template<typename... T>
  void print_error(const etl::string_view& from, T... args)
  {
    return kernel::debug::print_error(from, args...);
  }

  /**
   * \brief Возвращает пользовательские команды отладки
   * \param[out] to Строка, в которую будет скопирована пользовательская
   * команда отладки
   * \details
   * Пользовательская команда должна начинаться с "u " или "user ". Указанные
   * строки не включаются в возвращаемую строку.
   * 
   * Пример создания строки для получения команды отладки:
   * \code{.cpp}
   * etl::string<20> str;
   * \endcode
   * где 20 - максимальное кол-во символов в строке.
   * 
   * Если в буфере нет пользовательских команд, строка to будет пустой.
   * 
   * Для работы функции необходимо создать и выполнить как минимум один ФБ
   * \ref DebugPort.
   */
  inline void scan_debug(etl::istring& to)
  {
    return kernel::debug::scan(to);
  }

  /**
   * \}
   * \noop plc_com_debug
   */
}