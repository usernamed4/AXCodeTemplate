/**
 * \file kernel/kernel_api_serial.h
 * \brief API задач прямого доступа к последовательному порту
 * \ingroup kernel_api_serial
 * \details
 * Содержит запросы приема и передачи по последовательному порту
 */

#pragma once

#include "kernel/kernel_requests.h"
#include "etl/variant.h"


/**
 * \brief API прямого доступа к последовательному порту
 * \ingroup kernel_api_serial
 */
namespace kernel::serial::api
{
  /**
   * \addtogroup kernel_api_serial
   * \{
   */

  /**
   * \brief Пустой запрос
   * \details
   * Используется как флаг отсутствия запроса.
   */
  struct RequestDataNone
  {
  };

  //! Данные запроса на отправку посылки
  struct RequestDataSendBase
  {
    const etl::span<const char> send_buffer; //!< Буфер отправки

    /**
     * \brief Конструктор запроса
     * \param send_buffer Буфер отправки
     */
    constexpr RequestDataSendBase(const etl::span<const char>& send_buffer)
    : send_buffer{send_buffer}
    {
    }
  };

  /**
   * \brief Общие данные запросов на прием посылки
   * \details
   * Содержит список переменных, необходимых для выполнения любого запроса
   * на прием.
   * 
   * Новые запросы приема должны наследоваться от данной структуры
   */
  struct RequestDataReceiveBase
  {
    //! Буфер приема
    etl::span<char> receive_buffer{};
    //! Время ожидания выполнения запроса (мс), "0" - максимальное время
    uint32_t timeout{0};
    //! Пауза перед началом приема (мс), "0" - без паузы
    uint32_t delay{};
    //! Флаг выполнения условия окончания приема (выходной параметр)
    bool condition_executed{false}; 

  protected:
    /**
     * \brief Конструктор общих данных запроса на прием посылки
     * \param receive_buffer Буфер приема
     * \param timeout Время ожидания выполнения запроса (мс),
     * "0" - максимальное время
     * \param delay Пауза перед началом приема (мс), "0" - без паузы
     */
    constexpr RequestDataReceiveBase(
      const etl::span<char>& receive_buffer, uint32_t timeout, uint32_t delay
    )
    : receive_buffer{receive_buffer},
      timeout{timeout},
      delay{delay}
    {
    }
  };

  /**
   * \brief Данные запроса на прием до заполнения буфера приема
   * \details
   * Принимает данные до тех пор, пока не будет полностью заполнен
   * \ref receive_buffer.
   * 
   * По завершению приема в переменной \ref receive_buffer будет указывать
   * на фактически принятую посылку (не более изначального размера буфера).
   * 
   * Установит флаг \ref condition_executed если прием остановлен по заполнению
   * буфера.
   */
  struct RequestDataReceiveUntilFull : public RequestDataReceiveBase
  {
    /**
     * \brief Конструктор данных запроса
     * \param receive_buffer Буфер приема
     * \param timeout Время ожидания выполнения запроса (мс),
     * "0" - максимально возможное время
     * \param delay Пауза перед началом приема (мс), "0" - без паузы
     */
    constexpr RequestDataReceiveUntilFull(
      const etl::span<char>& receive_buffer, uint32_t timeout, uint32_t delay=0
    )
    : RequestDataReceiveBase{receive_buffer, timeout, delay}
    {
    }
  };

  /**
   * \brief Данные запроса на прием до считывания символа остановки приема
   * \details
   * Принимает данные до тех пор, пока не будет принят символ \ref end_symbol.
   * При этом сам символ остается в буфере приема.
   * 
   * По завершению приема в переменной \ref receive_buffer будет указывать
   * на фактически принятую посылку (не более изначального размера буфера).
   * 
   * Установит флаг \ref condition_executed если прием остановлен по символу
   */
  struct RequestDataReceiveUntilSymbol : public RequestDataReceiveBase
  {
    //! Символ остановки приема
    const char end_symbol{'\0'};

    /**
     * \brief Конструктор запроса
     * \param receive_buffer Буфер приема
     * \param end_symbol Символ остановки приема
     * \param timeout Время ожидания выполнения запроса (мс),
     * "0" - максимально возможное время
     * \param delay Пауза перед началом приема (мс), "0" - без паузы
     */
    constexpr RequestDataReceiveUntilSymbol(
      const etl::span<char>& receive_buffer,
      const char end_symbol,
      uint32_t timeout,
      uint32_t delay=0
    )
    : RequestDataReceiveBase{receive_buffer, timeout, delay},
      end_symbol{end_symbol}
    {
    }
  };

  /**
   * \brief Данные запроса на прием до считывания строки остановки приема
   * \details
   * Принимает данные до тех пор, пока конец принятых данных не совпадет
   * со строкой окочания приема \ref end_string. При этом сама строка остается
   * в буфере приема.
   * 
   * По завершению приема в переменной \ref receive_buffer будет указывать
   * на фактически принятую посылку (не более изначального размера буфера).
   * 
   * Установит флаг \ref condition_executed если прием остановлен по строке
   */
  struct RequestDataReceiveUntilString : public RequestDataReceiveBase
  {
    //! Строка окончания приема
    const etl::span<const char> end_string;

    /**
     * \brief Конструктор данных запроса
     * \param receive_buffer Буфер приема
     * \param end_string Строка окончания приема
     * \param timeout Время ожидания выполнения запроса (мс),
     * "0" - максимально возможное время
     * \param delay Пауза перед началом приема (мс), "0" - без паузы
     */
    constexpr RequestDataReceiveUntilString(
      const etl::span<char>& receive_buffer,
      const etl::span<const char>& end_string,
      uint32_t timeout,
      uint32_t delay=0
    )
    : RequestDataReceiveBase{receive_buffer, timeout, delay},
      end_string{end_string}
    {
    }
  };

  /**
   * \brief Данные запроса на прием до паузы после последнего приема посылки
   * \details
   * Принимает данные до тех пор, пока не пройдет время паузы \ref pause_time
   * с момента приема последнего байта.
   * 
   * По завершению приема в переменной \ref receive_buffer будет указывать
   * на фактически принятую посылку (не более изначального размера буфера).
   * 
   * Установит флаг \ref condition_executed если прием остановлен по времени
   * паузы
   */
  struct RequestDataReceiveUntilPause : public RequestDataReceiveBase
  {
    //! Время паузы после приема последнего байта (мс)
    const uint32_t pause_time;

    /**
     * \brief Конструктор данных запроса
     * \param receive_buffer Буфер приема
     * \param pause_time Время паузы после приема последнего байта (мс)
     * \param timeout Время ожидания выполнения запроса (мс),
     * "0" - максимально возможное время
     * \param delay Пауза перед началом приема (мс), "0" - без паузы
     */
    constexpr RequestDataReceiveUntilPause(
      const etl::span<char>& receive_buffer,
      const uint32_t pause_time,
      uint32_t timeout,
      uint32_t delay=0
    )
    : RequestDataReceiveBase{receive_buffer, timeout, delay},
      pause_time{pause_time}
    {
    }
  };

  /**
   * \brief Тип данных запроса на отправку
   * \details
   * Содержит список всех возможных запросов на отправку.
   * 
   * Для создания запроса необходимо в переменную типа данных запроса
   * на отправку присвоить объект запроса нужного типа (вызвать конструктор),
   * например:
   * \code{.cpp}
   * RequestDataSend request_send = RequestDataSendBase(buffer);
   * \endcode
   */
  using RequestDataSend = etl::variant<
    RequestDataNone,
    RequestDataSendBase
  >;

  /**
   * \brief Тип данных запроса на прием
   * \details
   * Содержит список всех возможных запросов на отправку.
   * 
   * Для создания запроса необходимо в переменную типа данных запроса на прием
   * присвоить объект запроса нужного типа (вызвать конструктор), например:
   * \code{.cpp}
   * RequestDataReceive request_receive = RequestDataReceiveUntilFull(
   *   buffer, timeout, delay
   * );
   * \endcode
   */
  using RequestDataReceive = etl::variant<
    RequestDataNone,
    RequestDataReceiveUntilFull,
    RequestDataReceiveUntilSymbol,
    RequestDataReceiveUntilString,
    RequestDataReceiveUntilPause
  >;

  /**
   * \brief Запрос к задаче прямого доступа к порту
   * \details
   * Содержит данные для запросов на отправку и/или прием.
   * При наличии обоих типов данных одновременно сначала произойдет отправка
   * а после, в случае успешной отправки, будет запущен прием.
   * 
   * Запрос должен быть отправлен в очередь запросов задачи прямого доступа
   * к последовательному порту.
   */
  struct Request
  {
    request::Header<Request> header; //!< Заголовок запроса
    RequestDataSend    send;         //!< Данные запроса на отправку
    RequestDataReceive receive;      //!< Данные запроса на прием
  };

  /**
   * \}
   * \noop kernel_api_serial
   */
}