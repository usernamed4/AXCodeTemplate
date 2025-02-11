/**
 * \file kernel/kernel_requests.h
 * \brief Шаблон запросов для общения между задачами
 * \ingroup kernel_os_requests
 */

#pragma once

#include "kernel/kernel_base.h"
#include "kernel/kernel_queues.h"
#include "kernel/kernel_events.h"

/**
 * \brief Пространство имен запросов ядра
 * \ingroup kernel_os_requests
 */
namespace kernel::request
{
  /**
   * \addtogroup kernel_os_requests
   * \{
   */

  //! Состояния выполнения запроса
  enum STATE : events_bitmask_t
  {
    RESET    = 0x00, //!< Запрос не обрабатывался
    READY    = 0x01, //!< Запрос готов к обработке
    BUSY     = 0x02, //!< Запрос в процессе обработки
    DONE     = 0x04, //!< Запрос успешно выполнен
    TIMEOUT  = 0x08, //!< Вышло время выполнения запроса
    ERROR    = 0x10, //!< Ошибка выполнения запроса
    ALL      = 0x1F, //!< Битовая маска всех состояний запросов

    //! Битовая маска состояний, завершивших выполнение запроса
    ALL_RESULT = DONE | TIMEOUT | ERROR 
  };


  /**
   * \brief Хранилище статической памяти для \ref iRequest
   */
  struct iRequestMemoryStorage : public EventsMemory
  {
  };


  /**
   * \brief Интерфейс статической памяти для \ref iRequest
   */
  struct iRequestMemoryInterface : public EventsMemoryInterface
  {
    /**
     * \brief Конструктор интерфейса статической памяти \ref iRequest
     * \param storage Ссылка на хранилище статической памяти, для которого
     * создается интерфейс
     */
    constexpr iRequestMemoryInterface(iRequestMemoryStorage& storage)
    : EventsMemoryInterface{static_cast<EventsMemoryStorage&>(storage)}
    {
    }
  };


  /**
   * \brief Статическая память для \ref iRequest
   */
  struct iRequestMemory
  : public iRequestMemoryStorage, public iRequestMemoryInterface
  {
    iRequestMemoryInterface& interface; //!< Интерфейс статической памяти

    /**
     * \brief Конструктор статической памяти для \ref iRequest
     */
    constexpr iRequestMemory()
    : iRequestMemoryStorage{},
      iRequestMemoryInterface{*static_cast<iRequestMemoryStorage*>(this)},
      interface{*this}
    {
    }
  };


  /**
   * \brief Интерфейс данных запроса
   * \details
   * Предназначен для передачи запроса по указателю
   * (например, через \ref kernel_os_queue).
   * 
   * Обратная связь о состоянии выполнения запроса считывается из переменной
   * \ref iRequest::state
   * 
   * \todo Добавить команду остановки выполнения запроса
   */
  class iRequest
  {
  public:
    //! Тип интерфейса памяти запроса
    using iMemory = iRequestMemoryInterface; 
    //! Тип хранилища статической памяти запроса
    using Memory  = iRequestMemory;
    //! Состояния выполнения запроса
    using STATE   = request::STATE;

  private:
    iMemory* _memory; //!< Память запроса

    /**
     * \brief Состояние запроса
     * \details
     * Может принимать одно из значений \ref STATE
     */
    kernel::StateEvents _state; 

  public:

    /**
     * \brief Конструктор интерфейса данных запроса
     * \param memory Интерфейс памяти запроса
     * (nullptr - для динамического выделения памяти)
     */
    constexpr iRequest(iMemory* memory=nullptr)
    : _memory{memory},
      _state{memory}
    {
    }


    /**
     * \brief Возвращает события состояния запроса
     * \details
     * const используется чтобы нельзя было изменить состояние запроса
     * вручную. Состояние меняется только через методы класса запроса
     */
    const kernel::StateEvents& state() const { return _state; }


    /**
     * \brief Проверяет создан ли запрос в операционной системе
     * \details
     * Проверяет создание объектов операционной системы класса интерфейса
     * запроса и объектов пользовательской обработки запроса, если реализованы
     * (вызывает метод \ref __isCreated_Impl()).
     * 
     * \retval true  Все объекты запроса созданы
     * \retval false Часть или все объекты не созданы, запрос не готов к работе
     */
    bool isCreated() const;

    /**
     * \brief Создает запрос в операционной системе
     * \details
     * Устанавливает состояние запроса 
     */
    bool create();

    /**
     * \brief Интерфейсный метод обработки запроса
     * \details
     * Выполняет обработку запроса.
     * 
     * При первом запуске (состояние запроса \b STATE::READY)
     * переключает запрос в состояние \b STATE::BUSY.
     * 
     * Выполняет пользовательский метод обработки запроса \ref __process_Impl()
     * и устанавливает в состоянии запроса \ref _state возвращемое 
     * пользовательским методом значение (см. описание к методу).
     * 
     * Вызов метода может происходить до тех пор, пока состояние запроса
     * равню \b STATE::BUSY или \b STATE::READY. В остальных случаях
     * считается, что обработка запроса завершена, и повторный вызов метода
     * переведет запрос в состояние ошибки \b STATE::ERROR.
     * 
     * \return Состояние выполнения запроса
     */
    STATE process();

  private:
    /**
     * \brief Проверка на создание всех объектов в реализации запроса
     * \details
     * Метод должен быть реализован пользователем библиотеки, если в запросе
     * присутствуют объекты операционной системы, которые должны быть созданы
     * для выполнения запроса
     * 
     * \retval true  Все необходимые объекты созданы
     * \retval false Часть или все объекты не созданы, запрос не готов к работе
     */
    virtual bool __isCreated_Impl() const
    {
      return true;
    }


    /**
     * \brief Создание объектов реализации запроса
     * \details
     * Метод должен быть реализован пользователем библиотеки, если в запросе
     * присутствуют объекты операционной системы, которые должны быть созданы
     * для выполнения запроса
     * 
     * \retval true  Объекты успешно созданы, либо создание не требуется
     * \retval false Ошибка создания объектов реализации запроса
     */
    virtual bool __create_Impl()
    {
      return true;
    }


    /**
     * \brief Реализация обработки запроса
     * \details
     * Содержит пользовательскую логику выполнения запроса.
     * 
     * Должен вернуть одно из указанных для метода возвращаемых значений,
     * иначе будет установлена ошибка \b STATE::ERROR.
     * 
     * \retval STATE::BUSY Запрос выполняется, требуется несколько циклов
     * работы задачи обработки запроса
     * \retval STATE::DONE Запрос выполнен (ошибки выполнения запроса, 
     * не вызванные выполнением функций запроса, тоже считаются выполненным
     * запросом и должны обрабатываться отдельно)
     * \retval STATE::TIMEOUT Вышло время выполнения запроса
     * \retval STATE::ERROR Системная ошибка выполнения запроса (ошибка 
     * использования функций, а не обработки запроса)
     */
    virtual STATE __process_Impl()
    {
      return STATE::ERROR;
    }
  };

  
  /**
   * \brief Заголовок запроса с ответами через очередь
   * \tparam QUEUE_ITEM Тип объекта запроса
   * \details
   * Используется для запросов, которые отправляют в качестве ответа
   * копию запроса, а не устанавливают битовую маску событий. Такой подход
   * занимает больше оперативной памяти, поэтому рекомендуется 
   * использование \ref iRequest
   */
  template<typename QUEUE_ITEM>
  struct Header
  {
    //! Состояние запроса
    STATE              state    {STATE::RESET};
    //! Очередь ответа
    Queue<QUEUE_ITEM>* response {nullptr};

    /**
     * \brief Конструктор заголовка запроса
     * \param response Очередь ответа
     */
    constexpr Header(Queue<QUEUE_ITEM>* response=nullptr)
    : response{response}
    {
    }
  };

  /**
   * \}
   * \noop kernel_os_requests
   */
}
