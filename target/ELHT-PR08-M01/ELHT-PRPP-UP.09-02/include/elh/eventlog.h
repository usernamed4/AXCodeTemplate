/**
 * \file elh/eventlog.h
 * \brief Модуль ведения журнала событий
 * \ingroup common_eventlog
 */

#pragma once

#include <cstdint>
#include "etl/algorithm.h"

#if defined(__has_include)
  #if __has_include("eventlog_table.h")
    #include "eventlog_table.h"
  #endif
#endif


/**
 * \brief Журнал событий
 * \ingroup common_eventlog 
 */
namespace elh::eventlog
{
  /**
   * \addtogroup common_eventlog
   * \{
   */

  using event_t = uint16_t;

  #define EVENTLOG_COMMON_SIZE 100
  #define EVENTLOG_IMPORTANT_SIZE 10

  /**
   * \brief Запись в лог
   */
  struct eventlog_record_t
  {
    event_t  code;
    event_t  line;
    uint32_t time;
  };
  void EventlogInit();
  void EventlogCallback(const eventlog_record_t& record);

  /**
   * \brief Добавляет событие в журнал
   * \param event_code Код события
   * \param line Строка, вызвавшая событие
   */
  eventlog_record_t EventlogWrite(event_t event_code, event_t line=0);

  /**
   * \brief Очищает журнал событий
   * \details
   * Удаляет записи о событиях полностью
   */
  void EventlogClear();

  /**
   * \brief Шаблон кольцевого буфера
   *
   * \tparam SIZE   Размер
   * \tparam DATA_T Тип данных ячейки буфера
   */
  template <class DATA_T, int SIZE>
  class CircularBuffer
  {
  public:
    //! Псевдоним для индексов
    typedef uint16_t INDEX_T;

  private:
    //! Память под буфер
    DATA_T _data[SIZE];

    //! Индекс записи
    volatile INDEX_T _writeIndex;

    //! Счетчик записей
    volatile INDEX_T _writeCount;

  public:
    //! Запись в буфер
    inline void write(const DATA_T& value)
    {
      if(_writeIndex >= SIZE)
      {
        _writeIndex = 0;
      }
      _data[_writeIndex] = value;
      if (++_writeIndex >= SIZE)
        _writeIndex = 0;

      if (_writeCount < SIZE){
        _writeCount++;
      }else{
        _writeCount = SIZE;
      }
    }

    //! возвращает элемент по индексу. 0-й - последний добавленный
    inline DATA_T &operator[](INDEX_T i)
    {
      DATA_T& ret = _data[(SIZE - 1 - i + _writeIndex) % SIZE];
      return ret;
    }

    inline const DATA_T operator[](INDEX_T i) const
    {
      DATA_T ret = _data[(SIZE - 1 - i + _writeIndex) % SIZE];
      return ret;
    }

    //! Пуст ли буфер
    inline bool isEmpty() const
    {
      return _writeCount == 0;
    }
    //! Полон ли буфер
    inline bool isFull() const
    {
      return _writeCount == SIZE;
    }
    //! Количество элементов в буфере
    INDEX_T size() const
    {
      return _writeCount;
    }

    //! Номер первого элемента в буфере
    INDEX_T index() const
    {
      return _writeIndex;
    }

    //! Очистить буфер
    inline void clear()
    {
      _writeIndex = 0;
      _writeCount = 0;
      etl::fill_n(_data, SIZE, DATA_T{});
    }
    //! размер буфера
    constexpr unsigned max_size() const
    {
      return SIZE;
    }

    //! Указатель на буфер данных
    const DATA_T* data() const
    {
      return _data;
    }

    //! Восстанавливает буфер по заданным параметрам
    void repair(
      uint16_t write_count, uint16_t write_index, const DATA_T* buffer
    ){
      _writeCount = (write_count <= SIZE) ? write_count : SIZE;
      _writeIndex = (write_index < SIZE) ? write_index : (SIZE - 1);
      etl::copy_n(buffer, SIZE, _data);
    }
  };


  /**
   * \}
   * \noop addtogroup common_eventlog
   */
}
