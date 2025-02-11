/**
 * \file kernel/kernel_queues.h
 * \brief Управление очередями ОСРВ
 * \ingroup kernel_os_queue
 * \details
 */

#pragma once

#include "kernel/kernel_base.h"
#include "queue.h"


namespace kernel
{
  /**
   * \addtogroup kernel_os_queue
   * \{
   */

  /**
   * \brief Общий интерфейс памяти для очередей с любым элементом
   */
  struct BaseQueueMemoryInterface
  {
  public:
    //! Ссылка на статическую память обработчика очереди FreeRTOS
    StaticQueue_t& queue_handler_memory; 
    //! Указатели на массив элементов очереди
    const etl::span<uint8_t> queue_items_memory; 
    //! Размер элемента очереди
    const size_t element_size;


    size_t max_size() const
    {
      return queue_items_memory.size() / element_size;
    }

  protected: 
    /**
     * \brief Конструктор общего интерфейса памяти очередей
     * \param queue_memory Ссылка на статическую память обработчика очереди
     * \param queue_items Указатели на начало и конец массива элементов очереди
     * \param element_size Размер элемента очереди
     */
    constexpr BaseQueueMemoryInterface(
      StaticQueue_t& queue_memory,
      etl::span<uint8_t> queue_items,
      size_t element_size
    )
    : queue_handler_memory {queue_memory},
      queue_items_memory   {queue_items},
      element_size         {element_size}
    {
    }
  };


  /**
   * \brief Интерфейс памяти очереди элемента типа T
   * \tparam T Тип элемента очереди
   */
  template<typename T>
  struct QueueMemoryInterface : public BaseQueueMemoryInterface
  {
    /**
     * \brief Конструктор интерфейса памяти очереди элемента типа T
     * \param queue_memory Ссылка на статическую память обработчика очереди
     * \param queue_items Указатели на начало и конец массива элементов очереди
     */
    constexpr QueueMemoryInterface(
      StaticQueue_t& queue_memory, const etl::span<T> queue_items
    )
    : BaseQueueMemoryInterface{
        queue_memory,
        {
          reinterpret_cast<uint8_t*>(queue_items.data()),
          reinterpret_cast<uint8_t*>(queue_items.end())
        },
        sizeof(T)
      }
    {
    }
  };


  /**
   * \brief Статическое хранилище памяти, занимаемой очередью
   * \tparam T Тип элемента очереди
   * \tparam SIZE Кол-во элементов в очереди
   */
  template<typename T, size_t SIZE>
  struct QueueMemory
  {
    //! Статическая память обработчика очереди FreeRTOS
    StaticQueue_t       queue_handler_storage;
    //! Массив элементов очереди
    etl::array<T, SIZE> queue_items_storage;

    //! Интерфейс элементов очереди
    QueueMemoryInterface<T> interface;

    /**
     * \brief Конструктор статической памяти очереди
     */
    constexpr QueueMemory()
    : queue_handler_storage{},
      queue_items_storage{},
      interface{queue_handler_storage, queue_items_storage}
    {
    }
  };


  /**
   * \brief Базовый класс очереди ядра
   * \details
   * Содержит неизменяемый код для любой очереди ядра
   */
  class BaseQueue
  {
  protected:
    //! Обработчик очереди FreeRTOS
    QueueHandle_t _handler;
    //! Статическая память объекта очереди
    BaseQueueMemoryInterface* const _memory;

    /**
     * \brief Конструктор базового класса очереди ядра
     * \param static_memory Интерфейс статической памяти ядра
     * (nullptr - для динамического выделения памяти)
     */
    constexpr BaseQueue(BaseQueueMemoryInterface* static_memory)
    : _handler{},
      _memory{static_memory}
    {
    }


    //! Проверяет была ли создана очередь в операционной системе
    bool __isCreated() const
    {
      return _handler != nullptr;
    }

    //! Возвращает указатель на обработчик очереди FreeRTOS
    QueueHandle_t __getQueueHandler() const
    {
      return _handler;
    }


    bool __create(size_t, size_t);
    bool __send(const void*, TickType_t) const;
    bool __sendFromISR(const void*, BaseType_t*) const;
    bool __receive(void*, TickType_t) const;
    bool __receiveFromISR(void*, BaseType_t*) const;
    bool __overwrite(const void*) const;
    bool __peek(void*, TickType_t) const;
    bool __reset() const;
  };


  /**
   * \brief Очередь ядра
   * \tparam T Тип элемента очереди
   * \details
   * Обертка над Си-очередями FreeRTOS, хранящая данные о типе объекта.
   * FreeRTOS во многом построен на использовании приведения указателей
   * к типу void*. Это дает гибкости в применении языка СИ, однако заставляет
   * вручную следить за приведением типов.
   * 
   * Данная обертка следит за типами в очередях и за правильностью указателей,
   * чтобы ошибка в использовании очереди не приводила к ошибке операционной
   * системы.
   */
  template<typename T>
  class Queue : private BaseQueue
  {
  private:
    const size_t _max_size; //!< Размер очереди

  public:
    /**
     * \brief Конструктор очереди по указателю на хранилище статической памяти
     * \param static_memory Интерфейс статической памяти очереди
     * \param max_size Размер очереди при динамическом выделении памяти,
     * не используется при статическом выделении памяти
     * (берется из static_memory)
     */
    constexpr Queue(QueueMemoryInterface<T>* static_memory, size_t max_size=0)
    : BaseQueue{static_memory},
      _max_size{(static_memory) ? static_memory->max_size() : max_size}
    {
    }

    /**
     * \brief Конструктор очереди по ссылке на интерфейс статической памяти
     * \param memory Интерфейс статической памяти очереди
     */
    constexpr Queue(QueueMemoryInterface<T>& memory)
    : Queue{&memory}
    {
    }

    bool create();
    bool send(const T& from, TickType_t timeout) const;
    bool sendFromISR(
      const T& from, BaseType_t* pxHigherPriorityTaskWoken
    ) const;
    bool receive(T& to, TickType_t timeout) const;
    bool receiveFromISR(T& to, BaseType_t* pxHigherPriorityTaskWoken) const;
    bool overwrite(T& from) const;
    bool peek(T& to, TickType_t timeout) const;
    bool reset() const;

    //! Возвращает true, если очередь была создана (метод \ref create())
    bool isCreated() const { return __isCreated(); }

    //! Возвращает указатель на обработчик очереди FreeRTOS
    QueueHandle_t getQueueHandler() const { return __getQueueHandler(); }

    //! Возвращает размер очереди
    size_t max_size() const { return _max_size; }
  };


  /**
   * \}
   * \noop kernel_os_queue
   */


  /**
   * \brief Создает очередь в операционной системе
   * \tparam T Тип элемента очереди
   * \details
   * Метод должен вызываться один раз в программе перед использованием прочих
   * методов.
   * 
   * Использует функции \ref xQueueCreateStatic() или \ref xQueueCreate()
   * в зависимости от того, используется статическое или динамическое выделение
   * памяти. Также автоматически передает размер очереди и элемента.
   * 
   * \retval true  Очередь успешно создана
   * \retval false Произошла ошибка создания очереди
   */
  template<typename T>
  bool Queue<T>::create()
  {
    return __create(_max_size, sizeof(T));
  }


  /**
   * \brief Отправляет элемент в очередь
   * \tparam T Тип элемента очереди
   * \param[in] from Ссылка на отправляемый элемент
   * \param[in] timeout Время ожидания отправки в очередь (в тиках процессора)
   * \details
   * Пробует отправить в очередь элемент за время ожидания. Во время ожидания
   * задача, вызвашая метод очереди, стоит на паузе. Использует функцию
   * \ref xQueueSend() операционной системы FreeRTOS, но проверяет правильность
   * аргументов.
   * 
   * \note Метод запрещено использовать в прерывании.
   * В прерывании необходимо использовать метод \ref sendFromISR()
   * 
   * \retval true  Элемент успешно отправлен
   * \retval false Элемент не отправлен (в очереди не появилось свободного
   * места за время timeout)
   */
  template<typename T>
  bool Queue<T>::send(const T& from, TickType_t timeout) const
  {
    return __send(&from, timeout);
  }


  /**
   * \brief Отправляет элемент в очередь из прерывания
   * \tparam T Тип элемента очереди
   * \param[in] from Отправляемый элемент
   * \param[out] pxHigherPriorityTaskWoken Указатель на флаг необходимости
   * смены задачи при выходе из прерывания (если была разбужена более
   * высокоприортетная задача) или nullptr
   * \details
   * Отправляет элемент без ожидания времени таймаута в прерывании. Использует
   * функцию xQueueSendFromISR() операционной системы FreeRTOS, но проверяет
   * правильность аргументов.
   * 
   * \note Метод используется только в прерывании.
   * Для отправки вне прерывания должен использоваться метод \ref send().
   * 
   * Работа с аргументом pxHigherPriorityTaskWoken описана в разделе
   * \ref kernel_interrupts_section_contextSwitch
   * 
   * \retval true  Элемент успешно отправлен
   * \retval false Элемент не отправлен (очередь полная)
   */
  template<typename T>
  bool Queue<T>::sendFromISR(
    const T& from, BaseType_t* pxHigherPriorityTaskWoken
  ) const
  {
    return __sendFromISR(&from, pxHigherPriorityTaskWoken);
  }


  /**
   * \brief Получает элемент из очереди
   * \tparam T Тип элемента очереди
   * \param[out] to Переменная, в которую копируется полученный элемент
   * \param[in] timeout Время ожидания получения элемента из очереди
   * (в тиках процессора)
   * \details
   * Ждет появления элемента в очереди в течение времени ожидания. Не изменяет
   * значение в аргументе to, если элемент не получен. Использует функцию
   * \ref xQueueReceive() операционной системы FreeRTOS, но проверяет
   * правильность аргументов.
   * 
   * \note Метод запрещено использовать в прерывании.
   * В прерывании необходимо использовать метод \ref receiveFromISR()
   * 
   * \retval true  Элемент получен из очереди
   * \retval false Элемент не получен за время timeout
   */
  template<typename T>
  bool Queue<T>::receive(T& to, TickType_t timeout) const
  {
    return __receive(&to, timeout);
  }


  /**
   * \brief Получает элемент из очереди в прерывании
   * \tparam T Тип элемента очереди
   * \param[in] to Ссылка на переменную, в которую копируется полученный элемент
   * \param[out] pxHigherPriorityTaskWoken Указатель на флаг необходимости
   * смены задачи при выходе из прерывания (если была разбужена более
   * высокоприортетная задача) или nullptr 
   * \details
   * Проверяет наличие элемента из очереди и считывает его в момент вызова
   * функции. Не изменяет переменную to, если элемент не получен. Использует
   * функцию \ref xQueueReceiveFromISR(), но проверяет правильность аргументов.
   * 
   * \note Метод используется только в прерывании.
   * Для отправки вне прерывания должен использоваться метод \ref receive().
   * 
   * Работа с аргументом pxHigherPriorityTaskWoken описана в разделе
   * \ref kernel_interrupts_section_contextSwitch
   * 
   * \retval true  Элемент получен из очереди
   * \retval false Элемент не получен (очередь пустая)
   */
  template<typename T>
  bool Queue<T>::receiveFromISR(
    T& to, BaseType_t* pxHigherPriorityTaskWoken
  ) const
  {
    return __receiveFromISR(&to, pxHigherPriorityTaskWoken);
  }


  /**
   * \brief Перезаписывает элемент в очереди новым значением поверх текущего
   * \tparam T Тип элемента очереди
   * \param[in] from Новое значение элемента очереди
   * \details
   * Работает только для очередей размера 1 (для очередей типа Mailbox).
   * Использует функцию \ref xQueueOverwrite() операционной системы FreeRTOS,
   * но проверяет правильность аргументов.
   * 
   * \note Метод запрещено использовать в прерывании.
   * Версия метода для прерываний не реализована в классе. 
   * При необходимости можно использовать функцию операционной системы
   * \ref xQueueOverwriteFromISR(), обработчик очереди для которого можно
   * считать с помощью метода \ref getQueueHandler()
   * 
   * \retval true  Элемент очереди успешно перезаписан
   * \retval false Произошла ошибка при обработке перезаписи очереди
   */
  template<typename T>
  bool Queue<T>::overwrite(T& from) const
  {
    return __overwrite(&from);
  }


  /**
   * \brief Считывает элемент без его удаления из очереди
   * \tparam T Тип элемента очереди
   * \param[in] to Переменная, в которую копируется значение элемента очереди
   * \param[in] timeout Время ожидания получения элемента из очереди
   * \details
   * Ждет появления элемента в очереди в течение времени ожидания. Не изменяет
   * значение аргумента to, если элемент не было получен. Использует функцию
   * \ref xQueuePeek() операционной системы FreeRTOS, но проверяет правильность
   * аргументов.
   * 
   * \note Метод не должен использоваться в прерывании.
   * Версия метода для прерываний не реализована в классе.
   * При необходимости можно использовать функцию операционной системы
   * \ref xQueuePeekFromISR(), обработчик очереди для которого можно
   * считать с помощью метода \ref getQueueHandler()
   * 
   * \retval true  Элемент прочитан из очереди
   * \retval false Элемент не прочитан за время timeout (очередь пустая)
   */
  template<typename T>
  bool Queue<T>::peek(T& to, TickType_t timeout) const
  {
    return __peek(&to, timeout);
  }


  /**
   * \brief Сбрасывает очередь
   * \details
   * Обнуляет все элементы очереди и сбрасывает текущий размер очереди.
   * Использует функцию операционной системы FreeRTOS \ref xQueueReset(),
   * но проверяет правильность аргументов.
   * 
   * \retval true Очередь сброшена
   * \retval false Ошибка сброса очереди
   */
  template<typename T>
  bool Queue<T>::reset() const
  {
    return xQueueReset(_handler);
  }
}
