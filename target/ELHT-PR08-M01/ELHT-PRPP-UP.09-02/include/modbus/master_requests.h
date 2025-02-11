/**
 * \file modbus/master_requests.h
 * \brief Запросы к Modbus Master
 */

#pragma once

#include "modbus/modbus_base.h"
#include "modbus/modbus_device.h"

namespace modbus
{
  namespace request
  {
    /**
     * \brief Состояние запроса Modbus
     * \todo Выделить в \ref kernel_requests.h общую часть и вынести за
     * операционку, использовать ее для описания состояний
     */
    enum class State
    {
      NONE,       //!< Запрос не инициализирован
      PROCESSING, //!< Обработка запроса
      DONE,       //!< Запрос успешно обработан
      ERROR       //!< Ошибка обработки запроса
    };

    //! Тип запроса
    enum class ID
    {
      //! Пустой запрос
      NONE, 

      //! Чтение регистров в меню устройства библиотеки Devices
      READ,

      //! Запись регистров из меню устройства библиотеки Devices
      WRITE,

      //! Выполнение указанной функции Modbus
      EXECUTE_FUNCTION
    };

    //! Перечисление возможных целей запроса
    enum class TYPE
    {
      NONE,          //!< Нет цели
      POLL_CHANNEL,  //!< Канал опроса
      TABLE,         //!< Таблица
      TABLE_VARIABLE //!< Переменная в таблице
    };
  }

  //! Цель запроса
  struct Link
  {
    modbus::Device*      device{};         //!< Опрашиваемое устройство
    device::PollChannel* poll_channel{};   //!< Канал опроса устройства
    menu::TableVariable  table_variable{}; //!< Параметр таблицы для опроса
  };

  //! Дополнительные данные для типа запроса
  union IdData
  {
    uint8_t none;

    //! Данные для запроса \ref ID::EXECUTE_FUNCTION
    struct ExecuteFunction
    {
      uint8_t function; //!< Функция запроса
    } execute_function;
  };


  //! Входные параметры запроса Modbus Master
  struct Request_Input
  {
    //! Тип запроса
    request::ID id{request::ID::NONE};
    //! Тип цели запроса
    request::TYPE type{request::TYPE::NONE};
    //! Максимальное кол-во регистров в запросе
    uint16_t max_registers{};
    //! Время ожидания ответа в запросе, мс (0 - настройки из Modbus Master)
    uint16_t response_timeout{};
    //! Цель запроса
    Link link {.device {}};
    //! Дополнительные данные для типа запроса
    IdData id_data{};
  };


  //! Тело запроса Modbus Master
  struct Request : public Request_Input
  {
    using Input  = Request_Input;  //!< Входные параметры запроса
    using Link   = modbus::Link;   //!< Цель запроса
    using IdData = modbus::IdData; //!< Дополнительные данные для типа запроса

    //! Состояние запроса
    request::State state;

    //! Код ошибки при обработке ответа на запрос
    EXCEPTION_CODE exception;
    //! Время начала выполнения запроса
    uint32_t send_time;

    /**
     * \brief Конструктор запроса к устройству
     * \param init Входные параметры запроса
     */
    Request(const Input& init);

    /**
     * \brief Конструктор запроса к устройству
     * \param id Тип запроса
     * \param type Тип цели запроса
     * \param max_registers Максимальное кол-во регистров в запросе
     * \param link Цель запроса
     * \param id_data Дополнительные данные для запроса
     * \details
     * Конструктор для обратной совместимости, рекомендуется использовать
     * Request(const Input& init);
     */
    Request(
      request::ID id,
      request::TYPE type,
      const Link& link,
      uint16_t max_registers={},
      IdData id_data={}
    );

    /**
     * \brief Конструктор пустого запроса
     * \details
     * Используется для инициализации в памяти задач, обрабрабатывающих
     * запросы.
     * 
     * Создавать сами запросы стоит через другие конструкторы
     * 
     * \note Получение пустого запроса вызовет ошибку
     */
    Request();
  };


  bool operator==(Request&, Request&);
}