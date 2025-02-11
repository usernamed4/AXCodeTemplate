/**
 * \file kernel/kernel_events.h
 * \brief События операционной системы
 * \ingroup kernel_os_events
 */

#pragma once

#include "kernel/kernel_base.h"
#include "event_groups.h"


namespace kernel
{
  /**
   * \addtogroup kernel_os_events
   * \{
   */

  //! Тип переменной битовой маски событий
  using events_bitmask_t = EventBits_t;


  /**
   * \brief Хранилище статический памяти событий ядра
   */
  struct EventsMemoryStorage
  {
    //! Статическая память группы событий FreeRTOS
    StaticEventGroup_t event_handler_storage;
  };

  /**
   * \brief Интерфейс хранилища статической памяти событий ядра
   */
  struct EventsMemoryInterface
  {
    //! Ссылка на статическую память группы событий FreeRTOS
    StaticEventGroup_t& event_handler_memory;

    /**
     * \brief Конструктор интерфейса памяти событий ядра
     * \param storage Хранилище статической памяти событий ядра
     */
    constexpr EventsMemoryInterface(EventsMemoryStorage& storage)
    : event_handler_memory{storage.event_handler_storage}
    {
    }
  };


  /**
   * \brief Статическая память событий ядра
   * \details
   * Объединяет в себе структуры интерфейса и хранилища пямяти
   */
  struct EventsMemory : public EventsMemoryStorage, public EventsMemoryInterface
  {
    EventsMemoryInterface& interface; //!< Интерфейс памяти событий ядра

    /**
     * \brief Конструктор статической памяти 
     */
    constexpr EventsMemory()
    : EventsMemoryStorage{},
      EventsMemoryInterface{*static_cast<EventsMemoryStorage*>(this)},
      interface{*this}
    {
    }
  };


  /**
   * \brief События ядра
   * \details
   * Обертка над группой событий FreeRTOS со статическим выделением памяти
   */
  class Events
  {
  public:
    using iMemory = EventsMemoryInterface; //!< Интерфейс памяти событий
    using Memory  = EventsMemory;          //!< Память событий

  private:
    EventsMemoryInterface* _memory;     //!< Память объекта событий
    EventGroupHandle_t _events_handler; //!< Обработчик событий FreeRTOS

  public:
    //! Перечисление значений битовой маски
    enum BITMASK : events_bitmask_t
    {
    #if configUSE_16_BIT_TICKS == 1
      BITMASK_ALL = 0x00FF     //!< 8 бит маски событий доступно
    #else
      BITMASK_ALL = 0x00FFFFFF //!< 24 бита маски событий доступно
    #endif
    };


    /**
     * \brief Конструктор событий по указателю на память
     * \param memory Указатель на память событий ядра
     * \todo Динамическое выделение пока не поддерживается
     */
    constexpr Events(EventsMemoryInterface* memory=nullptr)
    : _memory{memory},
      _events_handler{nullptr}
    {
    }


    /**
     * \brief Конструктор событий по ссылке на память
     * \param memory Статическая память объекта событий
     */
    constexpr Events(EventsMemoryInterface& memory)
    : Events{&memory}
    {
    }


    /**
     * \brief Создает объект событий в операционной системе
     * \details
     * Нужно обязательно вызвать метод один раз перед использованием прочих
     * методов.
     * 
     * Использует для создания объекта:
     * - При статическом выделения памяти:
     *   - Функцию \ref xEventGroupCreateStatic() операционной системы FreeRTOS
     *   - Статически выделенную память объекта событий
     * 
     * \retval true Объект событий создан в операционнной системе
     * \retval false Ошибка создания объекта событий в операционной системе
     */
    bool create();

    /**
     * \brief Ожидает появления указанных флагов
     * \param bitmask Битовая маска ожидаемых флагов
     * \param clear_on_exit Очистит ожидаемые флаги при выходе из функции
     * \param wait_for_all_bits Ожидание всех указанных в маске флагов
     * (true - функция работает по условию "И", false - условие "ИЛИ")
     * \param timeout Время ожидания в тиках операционной системы
     * \details
     * Переводит текущую задачу в режим ожидания на время таймаута или до
     * появления флагов ожидаемых событий.
     * 
     * \note Функция не должна использоваться в прерывании.
     * 
     * Аргументы метода совпадают с аргументами функции xEventGroupWaitBits()
     * для FreeRTOS, но метод дополнительно проверяет правильность аргументов.
     * 
     * \return Битовая маска флагов событий на момент установки ожидаемых флагов
     * или на момент окончания времени ожидания
     */
    events_bitmask_t waitBits(
      events_bitmask_t bitmask,
      bool clear_on_exit,
      bool wait_for_all_bits,
      uint32_t timeout
    ) const;

    /**
     * \brief Устанавливает флаги событий
     * \param bitmask Битовая маска устанавливаемых флагов
     * \details
     * Вызывает функцию \ref xEventGroupSetBits() операционной системы FreeRTOS,
     * но предварительно проверяет аргументы и состояние объекта событий.
     * 
     * \note Метод нельзя использовать в прерывании.
     * Для прерываний см. метод \ref setBitsFromISR()
     * 
     * \return Битовая маска флагов группы событий на момент выхода из
     * функции
     * \note Установка флага может вызывать более высокоприоритетную задачу,
     * которая обработает и сбросит флаг ожидаемого события. В таком случае
     * в возвращаемом значении этот флаг будет сброшен, а не установлен
     */
    events_bitmask_t setBits(events_bitmask_t bitmask);

    /**
     * \brief Устанавливает флаги событий в прерывании
     * \param[in]  bitmask Битовая маска устанавливаемых флагов
     * \param[out] pxHigherPriorityTaskWoken Опциональный флаг переключения
     * на более высокоприоритетную задачу при установке битов
     * \details
     * Вызывает функцию \ref xEventGroupSetBitsFromISR() операционной системы
     * FreeRTOS, но предварительно проверяет аргументы и состояние объекта
     * событий.
     * 
     * Флаги не устанавливаются мгновенно - метод добавляет установку флагов
     * в очередь команд высокоприоритетной системной задачи таймеров
     * операционной системы.
     * 
     * Если аргумент pxHigherPriorityTaskWoken не является нулевым указателем,
     * метод запишет в него \ref pdTRUE в случае необходимости смены задачи
     * при выходе из прерывания. В таком случае см. раздел
     * \ref kernel_interrupts_section_contextSwitch документации на ядро.
     * 
     * \note Метод должен использоваться в прерывании.
     * Вне прерывания см. метод \ref SetBits()
     * 
     * \retval true Новые флаги установлены
     * \retval false Новые флаги не установлены (очередь обработки флагов
     * занята)
     */
    bool setBitsFromISR(
      events_bitmask_t bitmask, BaseType_t *pxHigherPriorityTaskWoken
    );

    /**
     * \brief Сбрасывает флаги событий
     * \param bitmask Битовая маска сбрасываемых флагов
     * \details
     * Вызывает метод \ref xEventGroupClearBits() операционной системы FreeRTOS,
     * но предварительно проверяет правильность аргументов и состояние объекта
     * событий.
     * 
     * \note Метод нельзя использовать в прерывании. Для прерываний 
     * см. метод \ref clearBitsFromISR()
     * 
     * \return Битовая маска флагов событий до сброса указанных флагов
     */
    events_bitmask_t clearBits(events_bitmask_t bitmask);

    /**
     * \brief Сбрасывает флаги событий в прерывании
     * \param bitmask Битовая маска сбрасываемых флагов
     * \details
     * Вызывает метод \ref xEventGroupClearBitsFromISR() операционной системы,
     * но предварительно проверяет правильность аргументов и состояние объекта
     * событий.
     * 
     * События не сбрасываются мгновенно - метод добавляет сброс флагов
     * в очередь команд высокоприоритетной системной задачи таймеров
     * операционной системы.
     * 
     * \note Метод должен использоваться в прерывании.
     * Вне прерывания см. метод \ref clearBits()
     * 
     * \note Для сброса флагов не используется флаг вызова высокоприоритетной
     * задачи т.к. нет функций операционной системы, которые ждут сброса флагов,
     * а не их установки.
     * 
     * \retval true Успешно добавлена команда на сброс флагов событий
     * \retval false Ошибка отправки команды на сброс флагов (например, очередь
     * системной задачи обработки таймеров и команд занята)
     */
    bool clearBitsFromISR(events_bitmask_t bitmask);

    /**
     * \brief Возвращает текущее состояние флагов событий
     * \details
     * Вызывает метод \ref xEventGroupGetBits() операционной системы FreeRTOS,
     * но предварительно проверяет состояние объекта событий.
     * 
     * \note Метод не должен использоваться в прерывании.
     * Для прерываний см. метод \ref getBitsFromISR()
     * 
     * \return Битовая маска всей группы событий
     */
    events_bitmask_t getBits() const;

    /**
     * \brief Возвращает текущее состояние флагов событий в прерывании
     * \details
     * Вызывает метод \ref xEventGroupGetBitsFromISR() операционной системы
     * FreeRTOS, но предварительно проверяет состояние объекта событий.
     */
    events_bitmask_t getBitsFromISR() const;

    /**
     * \brief Проверяет создан ли объект событий в операционной системе
     * \details
     * Объект событий создается в операционной системе с помощью \ref create()
     * 
     * \retval true  Объект событий создан
     * \retval false Объект событий не создан
     */
    bool isCreated() const { return _events_handler != nullptr; }

    /**
     * \brief Возвращает указатель на объект группы событий FreeRTOS
     */
    EventGroupHandle_t event_handler() const { return _events_handler; }
  };


  /**
   * \brief События для отображения состояний только с одним активным
   * флагом одновременно
   * \details
   * Каждый флаг маски событий отображает отдельное состояние.
   * 
   * Одновременно может быть установлено только одно состояние в битовой маске
   */
  class StateEvents : protected Events
  {
  public:
    //! Интерфейс памяти событий состояний
    using iMemory = Events::iMemory;
    //! Память событий состояний
    using Memory  = Events::Memory;
    //! Конструктор событий состояний
    using Events::Events;
    //! Используется метод \ref create() для создания событий в FreeRTOS
    using Events::create;
    //! Возвращает текущее состояние флагов событий в прерывании
    using Events::getBitsFromISR;
    //! Возвращает true, если события были созданы (метод \ref create)
    using Events::isCreated;
    //! Возвращает указатель на обработчик группы событий FreeRTOS
    using Events::event_handler;

    /**
     * \brief Сбрасывает все флаги событий и устанавливает новое состояние
     * \param new_state Новое состояние (только один флаг битовой маски)
     * \details
     * Вызовет ошибку если:
     * - События не созданы в операционной системе
     * 
     * \note Метод нельзя использовать в прерывании.
     * \todo Добавить метод для работы в прерывании
     * 
     * \retval true Состояние установлено
     * \retval false Ошибка установки состояния
     */
    bool setState(events_bitmask_t new_state);

    /**
     * \brief Возвращает флаг текущего состояния событий
     * \details
     * Вызовет ошибку если:
     * - События не созданы в операционной системе
     * 
     * \note Метод нельзя использовать в прерывании.
     * \todo Добавить метод для работы в прерывании
     */
    events_bitmask_t getState() const;

    /**
     * \brief Ожидает установки указанных состояний
     * \param state Ожидаемые состояния (может быть несколько)
     * \param timeout Время ожидания
     * \return Текущее состояние на момент установки ожидаемых состояний
     * или на момент окончания времени ожидания
     */
    events_bitmask_t waitState(events_bitmask_t state, uint32_t timeout) const;
  };

  /**
   * \}
   * \noop addtogroup kernel_os_events
   */
}
