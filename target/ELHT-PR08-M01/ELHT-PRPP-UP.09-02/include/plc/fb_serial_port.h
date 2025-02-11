/**
 * \file fb_serial_port.h
 * \brief Функциональные блоки прямого доступа к COM-порту
 * \ingroup plc_com_serial
 */

#ifndef PLC_FB_SERIAL_PORT_H
#define PLC_FB_SERIAL_PORT_H

#include "fb_base.h"
#include "menu.h"
#include "kernel/kernel_api_serial.h"

namespace plc
{
  /**
   * \addtogroup plc_com_serial
   * \{
   */

  //! Тип данных перечисления COM-портов
  using COM = uint8_t;

  //! Проверка четности посылки
  enum COM_PARITY
  {
    COM_PARITY_NONE = 0, //!< Нет проверки четности
    COM_PARITY_ODD  = 1, //!< Нечетный
    COM_PARITY_EVEN = 2  //!< Четный
  };


  /**
   * \brief Список ошибок ФБ прямого доступа к порту
   * \details
   * Содержит список ошибок, характерных только для ФБ прямого доступа к порту
   * \note Список ошибок ФБ с номером менее 101  приведен в \ref FB_ERROR_CODE
   */
  enum SERIAL_ERROR_CODE
  {
    //! Буфер приема заполнен без выполнения условия остановки приема
    SERIAL_ERROR_CODE_RECEIVE_CONDITION_FAIL = 101
  };


  /**
   * \brief ФБ прямого доступа к последовательному порту
   * \details
   * - Открывает порт при инициализации
   * - Настраивает параметры связи
   * 
   * Для отправки и приема сообщений используются ФБ:
   * - \ref SerialPortWrite для отправки посылки
   * - \ref SerialPortRead для приема посылки
   * - \ref SerialPortWriteRead для отправки посылки и приема ответа
   */
  class SerialPort : public ETrig, protected component_link
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

  protected:
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
    };

  public:
    /**
     * \brief Конструктор ФБ прямого доступа к последовательному порту
     * \param init Структура инициализации входных параметров
     * \details
     * Структуру инициализации нужно поместить в фигурные скобки в аргументы
     * конструктора:
     * \code{.cpp}
     * SerialPort serial_port({
     *   .com        = COM1,
     *   .baudrate   = 115200,
     *   .parity     = COM_PARITY_NONE,
     *   .stop_bits  = 1
     * });
     * \endcode
     * 
     * Если в структуре инициализации не будет указан один из аргументов
     * (например, baudrate), то он примет значение по умолчанию из объявления
     * структуры Init.
     * 
     * Параметры конструктора должны идти в порядке, указанном в объявлении
     * структуры инициализации Init.
     */
    SerialPort(const Init& init);

  protected:
    inline static component_list<SerialPort> _list;
    TaskHandle_t _task;

    virtual FB_STATE processExecuting() override final;

    friend component_list<SerialPort>;
    friend class ComponentListHandler;
    friend class __SerialPortRequest;
  };


  /**
   * \brief Условие остановки приема по заполнению буфера приема
   */
  struct ReadStopConditionFull
  {

  };

  /**
   * \brief Условие остановки приема по получению стоп-символа
   */
  struct ReadStopConditionSymbol
  {
    //! Стоп-символ
    char stop_symbol{'\r'};

    //! Конструктор условия со стоп-символом по-умолчанию
    constexpr ReadStopConditionSymbol() =default;

    /**
     * \brief Конструктор условия с выбираемым символом
     * \param stop_symbol Стоп-символ
    */
    constexpr ReadStopConditionSymbol(const char stop_symbol)
    : stop_symbol{stop_symbol}
    {
    }
  };

  /**
   * \brief Условие остановки приема по совпадению со строкой в конце посылки
   * \details
   * \attention Строка должна существовать все время приема.
   * 
   * Для этого строка должна быть константной либо храниться в глобальной
   * переменной.
   * 
   * Пример создания условия с константной строкой:
   * \code{.cpp}
   * ReadStopConditionString condition("stop");
   * \endcode
   * 
   * Пример создания условия со строкой в глобальной переменной:
   * \code{.cpp}
   * etl::string<20> end_string{"stop"};
   * ReadStopConditionString condition(end_string);
   * \endcode
   */
  struct ReadStopConditionString
  {
    //! Указатель на буфер со строкой остановки приема
    etl::span<const char> stop_string{"\r"};

    //! Конструктор условия со строкой по-умолчанию
    constexpr ReadStopConditionString() =default;

    /**
     * \brief Конструктор условия с выбираемой строкой
     * \param stop_string Указатель на строку остановки приема
     * \attention Строка должна существовать все время приема. См. описание ФБ
     */
    constexpr ReadStopConditionString(const etl::span<const char>& stop_string)
    : stop_string{stop_string}
    {
    }
  };

  /**
   * \brief Условие остановки приема по паузе указанной длины после приема
   * посылки
   */
  struct ReadStopConditionPause
  {
    //! Время паузы после приема (мс)
    uint32_t pause_time{10};

    //! Конструктор условия с параметрами по-умолчанию
    constexpr ReadStopConditionPause() =default;
    
    /**
     * \brief Конструктор условия с паузой указанной длины
     * \param pause_time Время паузы после приема посылки (мс)
     */
    constexpr ReadStopConditionPause(const uint32_t pause_time)
    : pause_time{pause_time}
    {
    }
  };

  /**
   * \brief Конструктор условия, не требующего прием
   * \details
   * Данная структура предназначена для системного использования.
   */
  struct ReadStopConditionNone
  {
  };

  //! Список взоможных условий остановки приема
  using ReadStopCondition = etl::variant<
    ReadStopConditionNone,
    ReadStopConditionFull,
    ReadStopConditionSymbol,
    ReadStopConditionString,
    ReadStopConditionPause
  >;


  /**
   * \brief Системный ФБ отправки запросов в последовательный порт
   * \private
   * \details
   * Данный ФБ предназначен для системного использования.
   * Для работы с последовательным портом следует использовать:
   * - \ref SerialPortWrite для отправки посылки
   * - \ref SerialPortRead для приема посылки
   * - \ref SerialPortWriteRead для отправки посылки и приема ответа
   */
  class __SerialPortRequest : public ETrig
  {
  protected:
    SerialPort* _port;
    etl::span<const char> _send_buffer;
    etl::span<char> _read_buffer;
    ReadStopCondition _stop_condition;
    kernel::serial::api::Request _request;
    uint32_t _response_timeout;
    uint32_t _response_delay;

    __SerialPortRequest();

    kernel::QueueMemory<kernel::serial::api::Request, 1> _response_memory;
    kernel::Queue<kernel::serial::api::Request> _response;

    virtual FB_STATE processExecuting() override;
  };


  /**
   * \brief Функциональный блок отправки посылки через прямой доступ
   * к последовательному порту
   */
  class SerialPortWrite : public __SerialPortRequest
  {
  public:
    SerialPort*           port;        //!< ФБ прямого доступа к порту
    etl::span<const char> send_buffer; //!< Буфер отправки посылки

  protected:
    struct Init
    {
      SerialPort*           port {nullptr}; //!< ФБ прямого доступа к порту
      etl::span<const char> send_buffer {}; //!< Буфер отправки посылки
    };

  public:
    /**
     * \brief Конструктор ФБ отправки посылки
     * \param init Структура инициализации входных параметров
     * \details
     * Структуру инициализации нужно поместить в фигурные скобки в аргументы
     * конструктора:
     * \code{.cpp}
     * SerialPortWrite serial_port_write({
     *   .port        = &serial_port,
     *   .send_buffer = send_array // Например, char send_array[10]
     * });
     * \endcode
     * При инициализации допускается не указывать часть аргументов, но
     * в таком случае они должны быть записаны до выполнения ФБ.
     * 
     * Параметры конструктора должны идти в порядке, указанном в объявлении
     * структуры инициализации Init.
     */
    SerialPortWrite(const Init& init);

    /**
     * \brief Конструктор ФБ отправки посылки с параметрами по умолчанию
     */
    SerialPortWrite();

  private:
    virtual FB_STATE processExecuting() override final;
  };


  /**
   * \brief Функциональный блок приема посылки через прямой доступ
   * к последовательному порту
   */
  class SerialPortRead : public __SerialPortRequest
  {
  public:
    //! ФБ прямого доступа к порту
    SerialPort* port; 
    //! Буфер приема посылки
    etl::span<char> read_buffer;
    //! Условие завершения приема
    ReadStopCondition stop_condition; 
    //! Время ожидания ответа (мс). "0" - максимальное время ожидания
    uint32_t response_timeout;
    //! Время паузы перед началом приема (мс)
    uint32_t response_delay;

    //! Количество принятых байт в посылке
    size_t bytes_received() const;

    //! Принятая посылка в виде указателей на массив байт
    etl::span<char> data_received() const;

    //! Принятая посылка в виде указателей на строку
    etl::string_view string_received() const;

  protected:
    //! Структура инициализации функционального блока
    struct Init
    {
      //! ФБ прямого доступа к порту
      SerialPort* port {nullptr};
      //! Буфер приема посылки
      etl::span<char> read_buffer {};
      //! Условие завершения приема
      ReadStopCondition stop_condition{ReadStopConditionPause(10)}; 
      //! Время ожидания ответа (мс). "0" - максимальное время ожидания
      uint32_t response_timeout {1000};
      //! Время паузы перед началом приема (мс)
      uint32_t response_delay {0};
    };

  public:
    /**
     * \brief Конструктор ФБ приема посылки
     * \param init Структура инициализации входных параметров
     * \details
     * Структуру инициализации нужно поместить в фигурные скобки в аргументы
     * конструктора:
     * \code{.cpp}
     * SerialPortRead serial_port_read({
     *   .port             = &serial_port,
     *   .read_buffer      = read_array, // Например, char read_array[10]
     *   .stop_condition   = ReadStopConditionPause(10),
     *   .response_timeout = 1000,
     *   .response_delay   = 0
     * });
     * \endcode
     * При инициализации допускается не указывать port и read_buffer, но
     * в таком случае они должны быть записаны до выполнения ФБ.
     * 
     * stop_condition нужно указывать с аргументами конструктора
     * соответствующего условия.
     * 
     * Параметры конструктора должны идти в порядке, указанном в объявлении
     * структуры инициализации Init.
     */
    SerialPortRead(const Init& init);

    /**
     * \brief Конструктор ФБ приема посылки с параметрами по умолчанию
     */
    SerialPortRead();

  private:
    virtual FB_STATE processExecuting() override final;
  };


  /**
   * \brief Функциональный блок отправки и приема посылки через прямой доступ
   * к последовательному порту
   * \details
   * Отправляет посылку, а затем переключается на прием ответа
   */
  class SerialPortWriteRead : public __SerialPortRequest
  {
  public:
    //! ФБ прямого доступа к порту
    SerialPort* port;
    //! Буфер отправки посылки
    etl::span<const char> send_buffer;
    //! Буфер приема ответа
    etl::span<char> read_buffer;
    //! Условие завершения приема
    ReadStopCondition stop_condition;
    //! Время ожидания ответа (мс)
    uint32_t response_timeout;
    //! Время паузы между отправкой и приемом (мс)
    uint32_t response_delay;

    //! Количество принятых байт в посылке
    size_t bytes_received() const;

    //! Принятая посылка в виде указателей на массив байт
    etl::span<char> data_received() const;

    //! Принятая посылка в виде указателей на строку
    etl::string_view string_received() const;

  protected:
    //! Структура инициализации функционального блока
    struct Init
    {
      //! ФБ прямого доступа к порту
      SerialPort* port {nullptr}; 
      //! Буфер отправки посылки
      etl::span<const char> send_buffer{};
      //! Буфер приема посылки
      etl::span<char> read_buffer{};
      //! Условие завершения приема
      ReadStopCondition stop_condition{ReadStopConditionPause(10)};
      //! Время ожидания ответа (мс)
      uint32_t response_timeout {1000};
      //! Время паузы между отправкой и приемом (мс)
      uint32_t response_delay {0};
    };

  public:
    /**
     * \brief Конструктор ФБ отправки посылки и приема ответа
     * \param init Структура инициализации входных параметров
     * \details
     * Структуру инициализации нужно поместить в фигурные скобки в аргументы
     * конструктора:
     * \code{.cpp}
     * SerialPortWriteRead serial_port_writeread({
     *   .port             = &serial_port,
     *   .send_buffer      = send_array, // Например, char send_array[10]
     *   .read_buffer      = read_array, // Например, char read_array[10]
     *   .stop_condition   = ReadStopConditionPause(10),
     *   .response_timeout = 1000,
     *   .response_delay   = 0
     * });
     * \endcode
     * При инициализации допускается не указывать часть аргументов, но
     * в таком случае они должны быть записаны до выполнения ФБ.
     * 
     * stop_condition нужно указывать с аргументами конструктора
     * соответствующего условия.
     * 
     * Параметры конструктора должны идти в порядке, указанном в объявлении
     * структуры инициализации Init.
     */
    SerialPortWriteRead(const Init& init);

    /**
     * \brief Конструктор ФБ отправки посылки и приема ответа с параметрами 
     * по умолчанию
     */
    SerialPortWriteRead();
  
  private:
    virtual FB_STATE processExecuting() override final;
  };

  /**
   * \}
   * \noop plc_com_serial
   */
}

#endif /* PLC_FB_SERIAL_PORT_H */
